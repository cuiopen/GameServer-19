
#ifndef DBMYSQLCONNECTIONPOOL_H_
#define DBMYSQLCONNECTIONPOOL_H_

#include "DBConnectionPool.h"

namespace buf
{

class DBMysqlConnectionPool : public DBConnectionPool
{
public:
    DBMysqlConnectionPool(const char* url, int min = DBCP_MIN, int max = DBCP_MAX)
        : DBConnectionPool(url, min, max) {}
    DBMysqlConnectionPool(const std::string& url, int min  = DBCP_MIN, int max = DBCP_MAX)
        : DBConnectionPool(url, min, max) {}
    DBMysqlConnectionPool(const Url& url, int min  = DBCP_MIN, int max = DBCP_MAX)
        : DBConnectionPool(url, min, max) {}

    virtual ~DBMysqlConnectionPool() {}

    virtual bool initDescription();
    virtual DBConnection* createConnection();

private:
};

} // namespace buf

#endif // DBMYSQLCONNECTIONPOOL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

