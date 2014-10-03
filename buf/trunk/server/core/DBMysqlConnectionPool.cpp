
#include "DBMysqlConnection.h"
#include "DBMysqlConnectionPool.h"

namespace buf
{

DBConnection* DBMysqlConnectionPool::createConnection()
{
    if (!_dbd)
        return 0;
    DBConnection* conn = new DBMysqlConnection(_dbd);
    if (conn)
    {
        conn->connect(_url.host(), _url.user(),
                _url.passwd(), _url.db(), _url.port());
        return conn;
    }
    return 0;
}

bool DBMysqlConnectionPool::initDescription()
{
    if (_dbd)
        DELETENULL(_dbd);
    _dbd = DBMysqlConnection::createDescription(_url);
    return _dbd != 0;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

