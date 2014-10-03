
#ifndef DBQUERY_H_
#define DBQUERY_H_

#include "DBConnection.h"
#include "Buffer.h"
#include "RTime.h"
#if !defined(DB_SQL_MERGE)
#include "Logger.h"
#endif

namespace buf
{

class DBQuery
{
public:
    DBQuery(const char* sql, int len)
        : _data(len)
#if !defined(DB_SQL_MERGE)
          , _time("", &sLog)
#endif
    {
        if (sql && len)
            _data.put(sql, len);
    }
    DBQuery(const char* sql)
        : _data(buf::DB_ROW_LEN_MAX)
#if !defined(DB_SQL_MERGE)
          , _time("", &sLog)
#endif
    {
        if (sql)
            _data.put(sql, strlen(sql));
    }
    ~DBQuery() { _data.clear(); }

    const char* get() const { return _data.c_rbuf(); }
    Buffer::size_type size() const { return _data.rsize(); }

private:
    Buffer _data;
#if !defined(DB_SQL_MERGE)
    TimeFilm _time;
#endif
};

} // namespace buf

#endif // DBQUERY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

