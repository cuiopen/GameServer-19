
#ifndef DBCONNECTION_H_
#define DBCONNECTION_H_

#include "DBDescription.h"
#include "DBOHelper.h"
#include "Url.h"
#include "Buffer.h"

namespace buf
{
// XXX: return value of all functions are INTEGER

#define DB_ZIPMASK 0x80000000
#define DB_ZIPSET(l) (l|DB_ZIPMASK)
#define DB_ZIPCLEAR(l) (l&~DB_ZIPMASK)
#define DB_ZIPFLAG(l) (l&DB_ZIPMASK)
#define DB_ZIPLEN(l) (l&~DB_ZIPMASK)

#if defined(DEFAULT_SQL_LEN) && DEFAULT_SQL_LEN
static const unsigned int DB_ROW_LEN_MAX = DEFAULT_SQL_LEN;
#else
static const unsigned int DB_ROW_LEN_MAX = 64*1024;
#endif
static const unsigned int DB_LIMIT = 100;

static const char* DB_CHAR_SET_UTF8 = "utf8";
static const char* DB_CHAR_SET_GB2312 = "gb2312";
static const char* DB_CHAR_SET_GBK = "gbk";

typedef unsigned int dbbinlen_t;

enum DBConnectionState
{
    DBCS_INITED     = 0x01,
    DBCS_QUERYING   = 0x02,
    DBCS_CONNECTED  = 0x04,
    DBCS_PREPARING  = 0x08,
    DBCS_LOST       = 0x10
};

class DBConnection
{
public:
    DBConnection(DBDescription* dbd) : _dbd(dbd), _state(DBCS_INITED)
    {
        // XXX: avoid warning
        (void)DB_ROW_LEN_MAX;
        (void)DB_LIMIT;
        (void)DB_CHAR_SET_UTF8;
        (void)DB_CHAR_SET_GB2312;
        (void)DB_CHAR_SET_GBK;
    }
    virtual ~DBConnection() {}

    inline bool isInited() const { return _state & DBCS_INITED; }
    inline bool isQuerying() const { return _state & DBCS_QUERYING; }
    inline bool isConnected() const { return _state & DBCS_CONNECTED; }
    inline bool isPreparing() const { return _state & DBCS_PREPARING; }
    inline bool isLost() const { return _state & DBCS_LOST; }

    inline const char* host() const { return _host.c_str(); }
    inline const char* user() const { return _user.c_str(); }
    inline const char* passwd() const { return _passwd.c_str(); }
    inline const char* db() const { return _db.c_str(); }
    inline unsigned short port() const { return _port; }

    inline void setState(int state) { _state = state; }
    inline short getState() const { return _state; }

    inline bool connect(const Url& url)
    {
        _host = url.host();
        _user = url.user();
        _passwd = url.passwd();
        _db = url.db();
        _port = url.port();
        return connect(url.host(), url.user(), url.passwd(), url.db(), url.port());
    }

    virtual bool reconnect()
    {
        close();
        return connect(_host.c_str(), _user.c_str(), _passwd.c_str(), _db.c_str(), _port);
    }

    virtual bool connect(const char* host, const char* user,
            const char* passwd, const char* db, unsigned short port) = 0;
    virtual void close() = 0;

    virtual bool setCharset(const char* charset = "utf8") = 0;

    virtual int vquery(const char* sql, ...) = 0;
    virtual int query(const char* sql, int length, bool dropret = false) = 0;
    virtual int vquery(Buffer& result, const char* sql, ...) = 0;
    virtual int query(Buffer& result, const char* sql, int length) = 0;

    virtual int sqlSelect(
        const char* table,
            const char* columns,
                char* result,
                    int from = 0,
                        int limit = 1,
                            const char* where = 0,
                                const char* order = 0,
                                    const char* group = 0) = 0;

    virtual int sqlSelect(
        const char* table,
            const char* columns,
                Buffer& result,
                    int from = 0,
                        int limit = 1,
                            const char* where = 0,
                                const char* order = 0,
                                    const char* group = 0) = 0;

    virtual int sqlUpdate(
        const char* table,
            const char* columns,
                const char* value,
                    const char* where = 0) = 0;

    virtual int sqlInsert(
        const char* table,
            const char* columns,
                const char* value,
                    int rows = 1) = 0;

    virtual int sqlReplace(
        const char* table,
            const char* columns,
                const char* value,
                    int rows = 1) = 0;

    virtual int sqlDelete(const char* table, const char* where) = 0;

    virtual int count(const char* table, const char* where) = 0;
    virtual int commit() = 0;
    virtual int rollback() = 0;

    template <typename T>
    inline int prepare(const char* sql, T& v) { return prepare(sql, v._meta); }
    virtual int prepare(const char* sql, std::vector<DBOMeta>& meta) = 0;
    virtual int next() = 0;
    virtual void finish() = 0;

    virtual unsigned long escape(char* to, const char* from, size_t length) { strncpy(to, from, length); return length; };

    void escape(std::string& str)
    {
        if (str.empty())
            return;
        size_t length = str.length();
        char* buf = BUFNEW char[length*2+1];
        if (buf)
        {
            unsigned long ret = escape(buf, str.c_str(), length);
            if (ret)
                str = buf;
            DELETEVEC(buf);
        }
    }

protected:
    DBDescription* _dbd; // XXX: just a reference, it will be managed by other else
    short _state;
    std::string _host;
    std::string _user;
    std::string _passwd;
    std::string _db;
    unsigned short _port;
};

} // namespace buf

#endif // DBCONNECTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

