
#ifndef DBCONNECTIONPOOL_H_
#define DBCONNECTIONPOOL_H_

#include "DBConnection.h"
#include "DBQuery.h"
#include "Url.h"
#include "Mutex.h"
#include "ThreadGroup.h"

#include <deque>
#include <string>

namespace buf
{

class DBQueryThread;
#ifdef USE_QUERY_GROUP
class DBQueryThreadGroup;
static const int DBQT_MAX = 3;
#endif
// XXX: One pool manipulates one database

class DBConnectionPool
{
public:
    friend class DBQueryThread;

public:
    static const int DBCP_MIN = 3; // default minimal number of connections
    static const int DBCP_MAX = 12;// default max number of connections

public:
    DBConnectionPool(const char* url, int min = DBCP_MIN, int max = DBCP_MAX);
    DBConnectionPool(const std::string& url, int min  = DBCP_MIN, int max = DBCP_MAX);
    DBConnectionPool(const Url& url, int min  = DBCP_MIN, int max = DBCP_MAX);
    virtual ~DBConnectionPool();

    virtual DBConnection* createConnection() = 0;
    virtual bool initDescription() = 0;

    bool init();
    void join();
    bool hasConnection(const DBConnection* conn) const;

    inline const DBDescription* getDescription() const { return _dbd; }

    DBConnection* get(int status = DBCS_QUERYING);
    void put(DBConnection* conn);

    void vpush(const char* sql, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 2, 3)));
#else
        ;
#endif

    inline void push(const char* sql)
    {
        if (sql)
            push(sql, strlen(sql));
    }

    void push(const char* sql, int len);
    void push(std::string& sql);

    //void push(const DBQuery*, DBCallback);

protected:
#ifdef USE_QUERY_GROUP
    DBQueryThreadGroup* getWorker() { return _qwker; }
#else
    inline DBQueryThread* getWorker() { return _qwker; }
#endif
    DBConnection* newConnection();

protected:
    Url _url; // url of database
    int _min, _max, _curr, _free;
    DBDescription* _dbd;

    Mutex _clck;
    std::vector<DBConnection*> _cons;

#ifdef USE_QUERY_GROUP
    DBQueryThreadGroup* _qwker;
#else
    DBQueryThread* _qwker;
#endif
};

class DBQueryThread : public Thread
{
public:
    typedef std::vector<const DBQuery*> container;

public:
    DBQueryThread(const char* name, DBConnectionPool* pool, int tick = 5/*ms*/);
    virtual ~DBQueryThread();

    virtual void run();

    inline int getTick() const { return _tick; }
    inline void setTick(int tick) { _tick = tick; }

    void push(const DBQuery* query);

private:
    void flush(container& c, unsigned int s);

private:
    unsigned int _nquery;
    int _tick;
    Mutex _lock;
    container _queries;
    DBConnectionPool* _pool;
};

#ifdef USE_QUERY_GROUP
class DBQueryThreadGroup : public ThreadGroup
{
public:
    DBQueryThreadGroup(int max = DBQT_MAX)
        : ThreadGroup("DBQueryThreadGroup", max) {}
    ~DBQueryThreadGroup() { stop(); }

    inline void join() { joinAll(); }

    void push(const DBQuery* query);

    inline void start() { startAll(); }
    inline void stop() { stopAll(); }

private:
    const DBQueryThread* getQueryThread() const;
};
#endif


} // namespace buf

#endif // DBCONNECTIONPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

