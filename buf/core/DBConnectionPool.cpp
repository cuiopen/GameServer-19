
#include "DBConnectionPool.h"
#include "Logger.h"
#include <algorithm>
#include <stdarg.h>

namespace buf
{

DBConnectionPool::DBConnectionPool(const char* url, int min, int max)
: _url(url?url:"NULL"), _min(min), _max(max),
    _curr(min), _free(min), _dbd(0), _cons(max), _qwker(0)
{
    if (_max < _min)
    {
        std::swap(_min, _max);
        _curr = _min;
        _free = _min;
    }
}

DBConnectionPool::DBConnectionPool(const std::string& url, int min , int max)
: _url(url), _min(min), _max(max),
    _curr(min), _free(min), _dbd(0), _cons(max), _qwker(0)
{
    if (_max < _min)
    {
        std::swap(_min, _max);
        _curr = _min;
        _free = _min;
    }
}

DBConnectionPool::DBConnectionPool(const Url& url, int min , int max)
: _url(url), _min(min), _max(max),
    _curr(min), _free(min), _dbd(0), _cons(max), _qwker(0)
{
    if (_max < _min)
    {
        std::swap(_min, _max);
        _curr = _min;
        _free = _min;
    }
}

DBConnectionPool::~DBConnectionPool()
{
    if (_qwker)
    {
        _qwker->stop();
        _qwker->join();
    }
    DELETENULL(_dbd);
    DELETENULL(_qwker);
}

DBConnection* DBConnectionPool::newConnection()
{
    if (_curr >= _max)
        return 0;
    DBConnection* conn = createConnection();
    if (conn && conn->isConnected())
    {
        _cons[_curr] = conn;
        ++_curr;
        return conn;
    }
    DELETENULL(conn);
    return 0;
}

bool DBConnectionPool::init()
{
    if (!initDescription())
        return false;

    for (int i = 0; i < _min; ++i)
        newConnection();

#ifdef USE_QUERY_GROUP
    _qwker = new DBQueryThreadGroup();
    if (_qwker)
    {
        size_t size = _qwker->max_size();
        char name[32] = {0};
        for (size_t i = 0; i < size; ++i)
        {
            SNPRINTF(name, sizeof(name), "DBQueryThread-%lu", i);
            _qwker->put(new DBQueryThread(name, this));
        }
    }
#else
    _qwker = new DBQueryThread("DBQueryThread", this);
#endif
    if (_qwker)
        _qwker->start();
    return true;
}

DBConnection* DBConnectionPool::get(int status/* = DBCS_QUERYING*/)
{
    if (!_free)
        return 0;

    ScopeLock lock(_clck);
    DBConnection** conn = &_cons[0];
    for (int i = 0; i < _curr; ++i)
    {
        if (*conn && (*conn)->isConnected() && !(*conn)->isQuerying() && !(*conn)->isPreparing())
        {
            --_free;
            (*conn)->setState(status|DBCS_CONNECTED);
            return *conn;
        }
        ++conn;
    }

    if (_curr < _max)
        return newConnection();

    // TODO: log
    return 0;
}

void DBConnectionPool::join()
{
    if (_qwker)
        _qwker->join();
}

bool DBConnectionPool::hasConnection(const DBConnection* conn) const
{
    const DBConnection* const* c = &_cons[0];
    for (int i = 0; i < _curr; ++i)
    {
        if (*c && *c == conn)
            return true;
        ++c;
    }
    return false;
}

void DBConnectionPool::put(DBConnection* conn)
{
    if (!conn)
        return;
    ScopeLock lock(_clck);
    if (hasConnection(conn) && !conn->isLost())
    {
        conn->setState(DBCS_CONNECTED);
        ++_free;
        return;
    }
    // TODO: log
    DELETE(conn);
}

void DBConnectionPool::vpush(const char* sql, ...)
{
    char buffer[DB_ROW_LEN_MAX+1] = {0};
    va_list ap;
    va_start(ap, sql);
    int len = VSNPRINTF(buffer, sizeof(buffer), sql, ap);
    va_end(ap);
    if (len > 0)
        getWorker()->push(new DBQuery(buffer, len));
}

void DBConnectionPool::push(const char* sql, int len)
{
    if (!sql || !len)
        return;
    getWorker()->push(new DBQuery(sql, len));
}

void DBConnectionPool::push(std::string& sql)
{
    if (!sql.length())
        return;
    getWorker()->push(new DBQuery(sql.c_str(), sql.length()));
}

DBQueryThread::DBQueryThread(const char* name, DBConnectionPool* pool, int tick)
    : Thread(name?name:"Thread"), _nquery(0), _tick(tick), _pool(pool) {}

DBQueryThread::~DBQueryThread()
{
    // ScopeLock lock(_lock);
    if (_nquery)
        flush(_queries, _nquery);
    _pool = NULL;
}

void DBQueryThread::push(const DBQuery* query)
{
    if (!query || !_pool)
        return;

    ScopeLock lock(_lock);
    _queries.push_back(query);
    ++_nquery;
}

void DBQueryThread::flush(container& c, unsigned int s)
{
    const DBQuery** q = &c[0];
    DBConnection* conn = 0;
    unsigned int ss = s;

    do {
        conn = _pool->get();
        if (conn)
        {
            while (s)
            {
#if defined(DB_SQL_MERGE) && DB_SQL_MERGE > 1
                size_t left = 64*1024-1
                char buf[1024*64] = {0};
                unsigned int sss = 1;

                char pbuf = buf;
                while (s && sss%DB_SQL_MERGE)
                {
                    if (*q)
                    {
                        size_t slen = (*q)->size();
                        size_t sslen = slen + 1;
                        if (left < sslen)
                            break;

                        MEMCPY(pbuf, (*q)->get(), slen);
                        pbuf += slen;
                        pbuf = ';';
                        ++pbuf;

                        DELETE(*q);
                        left -= sslen;
                    }

                    --s;
                    ++q;
                    ++sss;
                }

                {
                    TimeFilm tf("SQL QUERY TIME: ", &sLog);
                    int ret = conn->query((char*)buf.rbuf(), buf.rsize(), true);
                    fprintf(stderr, "[%s][%u/%u][%u] %s => [%d]\n", getName(), s, ss, left, buf.rbuf(), ret);
                    sLog.trace("[%s][%u/%u][%u] %s => [%d]\n", getName(), s, ss, left, buf.rbuf(), ret);
                }
#else
                if (*q)
                {
                    if ((*q)->size())
                    {
                        int ret = conn->query((*q)->get(), (*q)->size(), true);
                        fprintf(stderr, "[%s][%u/%u] %s => [%d]\n", getName(), s, ss, (*q)->get(), ret);
                        sLog.trace("[%s][%u/%u] %s => [%d]\n", getName(), s, ss, (*q)->get(), ret);
                    }
                    DELETE(*q);
                }
                --s;
                ++q;
#endif
            }
            _pool->put(conn);
            break;
        }
    } while (!conn);
}

void DBQueryThread::run()
{
    if (!_pool)
        final();

    while (!isFinal())
    {
        if (_nquery)
        {
            std::vector<const DBQuery*> l;
            unsigned int nquery = 0;
            {
                ScopeLock lock(_lock);
                l = _queries;
                _queries.clear();
                nquery = _nquery;
                _nquery = 0;
            }

            flush(l, nquery);
        }

        Thread::usleep(_tick);
    }
}

#ifdef USE_QUERY_GROUP
const DBQueryThread* DBQueryThreadGroup::getQueryThread() const
{
    static unsigned long i = 1;
    return static_cast<const DBQueryThread*>((*this)[i++%size()]);
}

void DBQueryThreadGroup::push(const DBQuery* query)
{
    if (!query)
        return;

    const DBQueryThread* qt = getQueryThread();
    if (qt)
    {
        const_cast<DBQueryThread*>(qt)->push(query);
        return;
    }
    // TODO: log lost
    DELETENULL(query);
}
#endif

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

