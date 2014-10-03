
#include "Config.h" // for BUFNEW DELETEVEC
#include "DBMysqlConnection.h"
#include "Tokenizer.h"
#include "StringBuffer.h"
#include "Zipper.h"
#include "Unzipper.h"
#include "Buffer.h"
#include "Buffer.h"
#include "Thread.h"

#include <stdlib.h> // for ato*
#include <stdarg.h> // for va_*
#include <string.h> // for strchr
#include <errmsg.h> // for mysql_err*

namespace buf
{

DBMysqlConnection::DBMysqlConnection(DBDescription* dbd)
    : DBConnection(dbd), _stmt(0), _meta(0)
{
    mysql_thread_init();

    if (!mysql_init(&_mysql))
    {
        setState(DBCS_LOST);
        return;
    }

    bool reconnect = true;
    mysql_options(&_mysql, MYSQL_OPT_RECONNECT, &reconnect);
}

DBMysqlConnection::~DBMysqlConnection()
{
    close();
    mysql_thread_end();
}

DBDescription* DBMysqlConnection::createDescription(const Url& url)
{
    DBDescription* dbd = new DBDescription();
    if (!dbd)
        return 0;

    MYSQL mysql;
    mysql_thread_init();
    if (!mysql_init(&mysql))
    {
        mysql_thread_end();
        return 0;
    }

    bool reconnect = true;
    mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);

    if (!mysql_real_connect(&mysql, url.host(),
            url.user(), url.passwd(), url.db(), url.port(), 0, 0))
    {
        mysql_thread_end();
        return 0;
    }

    MYSQL_RES* res = mysql_list_tables(&mysql, 0);
    if (!res)
    {
        mysql_close(&mysql);
        mysql_thread_end();
        return 0;
    }

    MYSQL_ROW row = 0;
    while ((row = mysql_fetch_row(res)))
    {
        MYSQL_RES* fields = mysql_list_fields(&mysql, row[0], 0);
        if (fields)
        {
            DBTable* table = new DBTable(row[0], fields->field_count);
            DBColumn col;
            col.type = DB_UNKNOW;
            for (unsigned int i = 0; i < fields->field_count; ++i)
            {
                switch (fields->fields[i].type)
                {
                    case MYSQL_TYPE_TINY:
                        col.type = DB_TINY;
                        col.size = SIZEOF_TINYINT;
                        break;
                    case MYSQL_TYPE_SHORT:
                        col.type = DB_SHORT;
                        col.size = SIZEOF_SMALLINT;
                        break;
                    case MYSQL_TYPE_ENUM:
                    case MYSQL_TYPE_INT24:
                    case MYSQL_TYPE_LONG:
                        col.type = DB_LONG;
                        col.size = SIZEOF_INT;
                        break;
                    case MYSQL_TYPE_LONGLONG:
                        col.type = DB_LONGLONG;
                        col.size = SIZEOF_BIGINT;
                        break;
                    case MYSQL_TYPE_FLOAT:
                        col.type = DB_FLOAT;
                        col.size = SIZEOF_FLOAT;
                        break;
                    case MYSQL_TYPE_DOUBLE:
                        col.type = DB_DOUBLE;
                        col.size = SIZEOF_DOUBLE;
                        break;
                    case MYSQL_TYPE_NULL:
                        break;
                    case MYSQL_TYPE_TIMESTAMP:
                    case MYSQL_TYPE_DATE:
                    case MYSQL_TYPE_TIME:
                    case MYSQL_TYPE_DATETIME:
                    case MYSQL_TYPE_YEAR:
                    case MYSQL_TYPE_NEWDATE:
                        break;
                    case MYSQL_TYPE_SET:
                        break;
                    case MYSQL_TYPE_TINY_BLOB:
                    case MYSQL_TYPE_MEDIUM_BLOB:
                    case MYSQL_TYPE_LONG_BLOB:
                    case MYSQL_TYPE_BLOB:
                        col.type = DB_BLOBZ;
                        col.size = fields->fields[i].length;
                        break;
                    case MYSQL_TYPE_VARCHAR:
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                        col.type = DB_STRING;
                        col.size = fields->fields[i].length;
                        break;
                    case MYSQL_TYPE_GEOMETRY:
                        break;
                    case MYSQL_TYPE_BIT:
                        col.type = DB_LONGLONG;
                        col.size = SIZEOF_BIGINT;
                        break;
                    case MYSQL_TYPE_DECIMAL:
                    case MYSQL_TYPE_NEWDECIMAL:
                        col.type = DB_FLOAT;
                        col.size = SIZEOF_FLOAT; // XXX: ???
                        break;
                    default:
                        break;
                }

                col.name = fields->fields[i].name;
                table->addColumn(col);
            }
            dbd->addTable(table);
            table = 0;
        }
    }

    mysql_free_result(res);
    mysql_close(&mysql);
    mysql_thread_end();
    return dbd;
}

bool DBMysqlConnection::connect(
    const char* host,
        const char* user,
            const char* passwd,
                const char* db,
                    unsigned short port)
{
    if (isInited())
    {
        if (mysql_real_connect(&_mysql, host, user, passwd, db, port, 0, CLIENT_REMEMBER_OPTIONS|CLIENT_MULTI_STATEMENTS))
        {
            setState(DBCS_CONNECTED);
            setCharset(DB_CHAR_SET_UTF8);

            // XXX:
            vquery("SET NAMES `utf8`");
            vquery("SET CHARACTER SET `utf8`");
            return true;
        }
    }
    return false;
}

bool DBMysqlConnection::setCharset(const char* charset)
{
    if (isConnected())
    {
        mysql_set_character_set(&_mysql, charset); // XXX: 
        return true;
    }
    return false;
}

// XXX: must be called before pthread_exit
void DBMysqlConnection::close()
{
    if (isInited() || isQuerying() || isConnected())
    {
        mysql_close(&_mysql);
        setState(DBCS_INITED);
    }
}

int DBMysqlConnection::vquery(const char* sql, ...)
{
    if (!sql)
        return -1;
    if (!isQuerying())
        return -1;

    // XXX: out of range
    char buffer[DB_ROW_LEN_MAX+1] = {0};
    va_list ap;
    va_start(ap, sql);
    int len = VSNPRINTF(buffer, sizeof(buffer), sql, ap);
    va_end(ap);

    if (!len)
        return -1;

    return query(buffer, len);
}

int DBMysqlConnection::vquery(Buffer& result, const char* sql, ...)
{
    if (!sql)
        return -1;
    if (!isQuerying())
        return -1;

    // XXX: out of range
    char buffer[DB_ROW_LEN_MAX+1] = {0};
    va_list ap;
    va_start(ap, sql);
    int len = VSNPRINTF(buffer, sizeof(buffer), sql, ap);
    va_end(ap);

    if (!len)
        return -1;

    return query(result, buffer, len);
}

int DBMysqlConnection::query(const char* sql, int length, bool dropret)
{
    if (!sql || !length)
        return -1;
    if (!isQuerying())
        return -1;

    Buffer s((length<<1)+1);
    unsigned long len = escape(s.c_wbuf(), sql, length);
    s.wflip(len);

    do {
        int ret = mysql_real_query(&_mysql, s.c_rbuf(), s.rsize());
        if (!ret)
        {
            setState(DBCS_QUERYING|DBCS_CONNECTED);

            if (dropret)
            {
                do
                {
                    MYSQL_RES* res = mysql_store_result(&_mysql);
                    if (res)
                        mysql_free_result(res);
                } while (!mysql_next_result(&_mysql));
            }
            return 0;
        }

        switch (mysql_errno(&_mysql))
        {
            case CR_SOCKET_CREATE_ERROR:
            case CR_CONNECTION_ERROR:
            case CR_CONN_HOST_ERROR:
            case CR_IPSOCK_ERROR:
            case CR_SERVER_GONE_ERROR:
            case CR_SERVER_LOST:
            case CR_TCP_CONNECTION:
                {
                    fprintf(stderr, "%s\n", mysql_error(&_mysql));
                    setState(DBCS_LOST);
                }
                break;

            case CR_COMMANDS_OUT_OF_SYNC:
                {
                    // XXX: ugly
                    MYSQL_RES* res = mysql_store_result(&_mysql);
                    if (res)
                        mysql_free_result(res);
                }
                break;

            default:
                // TODO: error log
                break;
        }

        if (isLost())
        {
            // TODO: log
            fprintf(stderr, "Reconnecting to database...\n");
            Thread::sleep(1);
        }
    } while (isLost());

    return -1;
}

int DBMysqlConnection::query(Buffer& result, const char* sql, int length)
{
    int ret = query(sql, length);
    if (ret >= 0)
        return fetchRows(result);
    return ret;
}

// select [column].* from db.table [where stmt group by stmt order by stmt]
int DBMysqlConnection::sqlSelect(
    const char* table,
        const char* columns,
            char* result,
                int from,
                    int limit,
                        const char* where,
                            const char* order,
                                const char* group)
{
    if (!_dbd)
        return -1;

    if (!table || !columns || !limit)
        return -1;

    if (!isQuerying())
        return -1;

    const DBTable* dbt = _dbd->getTable(table);
    if (!dbt)
        return -1;

    const DBColumn* dbc = dbt->getColumns();
    if (!dbc)
        return -1;

    size_t cnum = dbt->getColumnNum();
    if (!cnum)
        return -1;

    StringBuffer sql;
    sql << "SELECT ";

    std::vector<DBColumn> dbcs;
    const DBColumn* pdbc = dbc;
    size_t ncnum = cnum;

    // *|col1,col2,...colN
    if (strchr(columns, '*'))
    {
        sql << "*";
    }
    else
    {
        Tokenizer tk(columns, ',');
        size_t size = tk.size();
        if (!size)
            return -1;

        const DBColumn* pcol = 0;
        for (size_t i = 0; i < size; ++i)
        {
            pcol = dbt->hasColumn(tk[i].c_str());
            if (!pcol)
            {
                // TODO: log,error
            }
            if (i && i < size)
                sql << ",`";
            else
                sql << "`";
            sql << tk[i] << "`";
            dbcs.push_back(*pcol);
        }

        pdbc = &dbcs[0];
        ncnum = dbcs.size();
    }
    sql << " FROM `";
    sql << dbt->getName() << "`"; // XXX:
    
    if (where)
        sql << " WHERE " << where;

    if (group)
        sql << " GROUP BY " << group;

    if (order)
        sql << " ORDER BY " << order;

    if (from || limit)
    {
        sql << " LIMIT " << from;
        if (!limit)
            limit = DB_LIMIT;
        sql << "," << limit;
    }
    else
    {
        sql << " LIMIT " << DB_LIMIT;
    }

    // XXX: LOCK???
    int ret = query(&sql, sql.rsize());
    if (!ret)
    {
        MYSQL_ROW row = 0;
        MYSQL_RES* res = mysql_store_result(&_mysql);
        MYSQL_FIELD* fields = mysql_fetch_fields(res);

        int rows = 0;
        char* pres = result;
        if (!mysql_field_count(&_mysql))
            return 0;
        unsigned int nfields = mysql_num_fields(res);
        if (nfields != ncnum)
            return -1;
        while ((row = mysql_fetch_row(res)))
        {
            ++rows;
            unsigned long* lengths = mysql_fetch_lengths(res);
            for (unsigned int i = 0; i < nfields; ++i)
            { // nfields == ncnum
                switch (fields[i].type)
                {
                    case MYSQL_TYPE_TINY:
                        if (row[i])
                            *reinterpret_cast<unsigned char*>(pres) = atoi(row[i]);
                        else
                            *reinterpret_cast<unsigned char*>(pres) = 0;
                        pres += sizeof(unsigned char);
                        break;
                    case MYSQL_TYPE_SHORT:
                        if (row[i])
                            *reinterpret_cast<unsigned short*>(pres) = atoi(row[i]);
                        else
                            *reinterpret_cast<unsigned short*>(pres) = 0;
                        pres += sizeof(unsigned short);
                        break;
                    case MYSQL_TYPE_LONG:
                        if (row[i])
                            *reinterpret_cast<unsigned int*>(pres) = atoi(row[i]);
                        else
                            *reinterpret_cast<unsigned int*>(pres) = 0;
                        pres += sizeof(unsigned int);
                        break;
                    case MYSQL_TYPE_LONGLONG:
                        if (row[i])
                            *reinterpret_cast<unsigned long long*>(pres) = atoll(row[i]);
                        else
                            *reinterpret_cast<unsigned long long*>(pres) = 0;
                        pres += sizeof(unsigned long long);
                        break;
                    case MYSQL_TYPE_INT24:
                        if (row[i])
                            *reinterpret_cast<unsigned int*>(pres) = atol(row[i]);
                        else
                            *reinterpret_cast<unsigned int*>(pres) = 0;
                        pres += sizeof(unsigned int);
                        break;
                    case MYSQL_TYPE_ENUM:
                        if (row[i])
                            *reinterpret_cast<unsigned int*>(pres) = atoi(row[i]);
                        else
                            *reinterpret_cast<unsigned int*>(pres) = 0;
                        pres += sizeof(int);
                        break;
                    case MYSQL_TYPE_FLOAT: // XXX: if float(p) when p > 25
                        if (row[i])
                            *reinterpret_cast<float*>(pres) = atof(row[i]);
                        else
                            *reinterpret_cast<float*>(pres) = 0.0;
                        pres += sizeof(float);
                        break;
                    case MYSQL_TYPE_DOUBLE:
                        if (row[i])
                            *reinterpret_cast<double*>(pres) = atof(row[i]);
                        else
                            *reinterpret_cast<double*>(pres) = 0.0;
                        pres += sizeof(double);
                        break;
                    case MYSQL_TYPE_NULL:
                        break;
                    case MYSQL_TYPE_TIMESTAMP:
                        break;
                    case MYSQL_TYPE_DATE:
                        break;
                    case MYSQL_TYPE_TIME:
                        break;
                    case MYSQL_TYPE_DATETIME:
                        break;
                    case MYSQL_TYPE_YEAR:
                        break;
                    case MYSQL_TYPE_NEWDATE:
                        break;
                    case MYSQL_TYPE_SET:
                        break;
                    case MYSQL_TYPE_TINY_BLOB:
                    case MYSQL_TYPE_MEDIUM_BLOB:
                    case MYSQL_TYPE_LONG_BLOB:
                    case MYSQL_TYPE_BLOB:
                        if (row[i])
                        {
                            dbbinlen_t* len = reinterpret_cast<dbbinlen_t*>(row[i]);
                            dbbinlen_t length = DB_ZIPLEN(*len);
                            if (DB_ZIPFLAG(*len) && length)
                            {
                                Unzipper unzipper((char*)(len+1), lengths[i]-sizeof(*len));
                                *reinterpret_cast<dbbinlen_t*>(pres) = length;
                                pres += sizeof(*len);
                                MEMCPY(pres, &unzipper, unzipper.size());
                                pres += unzipper.size();
                                break;
                            }
                            else if (!DB_ZIPFLAG(*len) && length)
                            {
                                *reinterpret_cast<dbbinlen_t*>(pres) = DB_ZIPLEN(*len);
                                pres += sizeof(*len);
                                MEMCPY(pres, reinterpret_cast<const char*>(len+1), lengths[i]-sizeof(*len));
                                pres += length;
                                break;
                            }
                        }
                        *reinterpret_cast<dbbinlen_t*>(pres) = 0;
                        pres += sizeof(dbbinlen_t);
                        break;
                    case MYSQL_TYPE_VARCHAR:
                    case MYSQL_TYPE_VAR_STRING:
                    case MYSQL_TYPE_STRING:
                        if (row[i])
                            MEMCPY(pres, row[i], lengths[i]);
                        // XXX: fill left memory with ZERO
                        MEMSET(pres+lengths[i], 0x00, pdbc[i].size-lengths[i]);
                        pres += pdbc[i].size;
                        break;
                    case MYSQL_TYPE_GEOMETRY:
                        break;
                    case MYSQL_TYPE_BIT:
                        break;
                    case MYSQL_TYPE_DECIMAL:
                        break;
                    case MYSQL_TYPE_NEWDECIMAL:
                        break;
                    default:
                        break;
                }
            }
        }

        mysql_free_result(res);
        return rows;
    }
    return -1;
}

int DBMysqlConnection::fetchRows(Buffer& result, size_t ncnum)
{
    MYSQL_ROW row = 0;
    MYSQL_RES* res = mysql_store_result(&_mysql); // XXX:
    MYSQL_FIELD* fields = mysql_fetch_fields(res);

    int rows = 0;
    if (!mysql_field_count(&_mysql))
        return 0;
    unsigned int nfields = mysql_num_fields(res);
    if (ncnum != static_cast<size_t>(-1) && nfields != ncnum)
        return -1;
    while ((row = mysql_fetch_row(res)))
    {
        ++rows;
        unsigned long* lengths = mysql_fetch_lengths(res);
        for (unsigned int i = 0; i < nfields; ++i)
        { // nfields == ncnum
            switch (fields[i].type)
            {
                case MYSQL_TYPE_TINY:
                    if (row[i])
                        result << static_cast<unsigned char>(atoi(row[i]));
                    else
                        result << static_cast<unsigned char>(0);
                    break;
                case MYSQL_TYPE_SHORT:
                    if (row[i])
                        result << static_cast<unsigned short>(atoi(row[i]));
                    else
                        result << static_cast<unsigned short>(0);
                    break;
                case MYSQL_TYPE_LONG:
                    if (row[i])
                        result << static_cast<unsigned int>(atoi(row[i]));
                    else
                        result << static_cast<unsigned int>(0);
                    break;
                case MYSQL_TYPE_LONGLONG:
                    if (row[i])
                        result << static_cast<unsigned long long>(atoll(row[i]));
                    else
                        result << static_cast<unsigned long long>(0);
                    break;
                case MYSQL_TYPE_INT24:
                    if (row[i])
                        result << static_cast<unsigned int>(atol(row[i]));
                    else
                        result << static_cast<unsigned int>(0);
                    break;
                case MYSQL_TYPE_ENUM:
                    if (row[i])
                        result << static_cast<unsigned int>(atoi(row[i]));
                    else
                        result << static_cast<unsigned int>(0);
                    break;
                case MYSQL_TYPE_FLOAT:
                    if (row[i])
                        result << static_cast<float>(atof(row[i]));
                    else
                        result << static_cast<float>(0.0);
                    break;
                case MYSQL_TYPE_DOUBLE:
                    if (row[i])
                        result << static_cast<double>(atof(row[i]));
                    else
                        result << static_cast<double>(0.0);
                    break;
                case MYSQL_TYPE_NULL:
                    break;
                case MYSQL_TYPE_TIMESTAMP:
                    break;
                case MYSQL_TYPE_DATE:
                    break;
                case MYSQL_TYPE_TIME:
                    break;
                case MYSQL_TYPE_DATETIME:
                    break;
                case MYSQL_TYPE_YEAR:
                    break;
                case MYSQL_TYPE_NEWDATE:
                    break;
                case MYSQL_TYPE_SET:
                    break;
                case MYSQL_TYPE_TINY_BLOB:
                case MYSQL_TYPE_MEDIUM_BLOB:
                case MYSQL_TYPE_LONG_BLOB:
                case MYSQL_TYPE_BLOB:
                    if (row[i])
                    {
                        dbbinlen_t* len = reinterpret_cast<dbbinlen_t*>(row[i]);
                        dbbinlen_t length = DB_ZIPLEN(*len);
                        if (DB_ZIPFLAG(*len) && length)
                        {
                            Unzipper unzipper((char*)(len+1), lengths[i]-sizeof(*len));
                            result << length;
                            result.put(&unzipper, unzipper.size());
                            break;
                        }
                        else if (!DB_ZIPFLAG(*len) && length)
                        {
                            result << DB_ZIPLEN(*len);
                            result.put(reinterpret_cast<Buffer::const_pointer>(len+1), lengths[i]-sizeof(*len));
                            break;
                        }
                    }
                    result << static_cast<dbbinlen_t>(0);
                    break;
                case MYSQL_TYPE_VARCHAR:
                case MYSQL_TYPE_VAR_STRING:
                case MYSQL_TYPE_STRING:
                    if (row[i])
                        result.put(row[i], lengths[i]);
                    // XXX: fill left memory with ZERO
                    MEMSET(result.wbuf(), 0x00, fields[i].length-lengths[i]);
                    break;
                case MYSQL_TYPE_GEOMETRY:
                    break;
                case MYSQL_TYPE_BIT:
                    break;
                case MYSQL_TYPE_DECIMAL:
                    break;
                case MYSQL_TYPE_NEWDECIMAL:
                    break;
                default:
                    break;
            }
        }
    }

    mysql_free_result(res);
    return rows;
}

int DBMysqlConnection::sqlSelect(
    const char* table,
        const char* columns,
            Buffer& result,
                int from,
                    int limit,
                        const char* where,
                            const char* order,
                                const char* group)
{
    // XXX: copied from sqlSelect with the type of result is char*
    if (!_dbd)
        return -1;

    if (!table || !columns || !limit)
        return -1;

    if (!isQuerying())
        return -1;

    const DBTable* dbt = _dbd->getTable(table);
    if (!dbt)
        return -1;

    const DBColumn* dbc = dbt->getColumns();
    if (!dbc)
        return -1;

    size_t cnum = dbt->getColumnNum();
    if (!cnum)
        return -1;

    StringBuffer sql;
    sql << "SELECT ";

    std::vector<DBColumn> dbcs;
    const DBColumn* pdbc = dbc;
    size_t ncnum = cnum;

    // *|col1,col2,...colN
    if (strchr(columns, '*'))
    {
        sql << "*";
    }
    else
    {
        Tokenizer tk(columns, ',');
        size_t size = tk.size();
        if (!size)
            return -1;

        const DBColumn* pcol = 0;
        for (size_t i = 0; i < size; ++i)
        {
            pcol = dbt->hasColumn(tk[i].c_str());
            if (!pcol)
            {
                // TODO: error,log
            }
            if (i && i < size)
                sql << ",`";
            else
                sql << "`";
            sql << tk[i] << "`";
            dbcs.push_back(*pcol);
        }

        pdbc = &dbcs[0];
        ncnum = dbcs.size();
    }
    sql << " FROM `";
    sql << dbt->getName() << "`"; // XXX:
    
    if (where)
        sql << " WHERE " << where;

    if (group)
        sql << " GROUP BY " << group;

    if (order)
        sql << " ORDER BY " << order;

    if (from || limit)
    {
        sql << " LIMIT " << from;
        if (!limit)
            limit = DB_LIMIT;
        sql << "," << limit;
    }
    else
    {
        sql << "`";
    }

    // XXX: LOCK???
    int ret = query(&sql, sql.rsize());
    if (!ret)
        return fetchRows(result, ncnum);
    return -1;
}

int DBMysqlConnection::sqlUpdate(
    const char* table,
        const char* columns,
            const char* value,
                const char* where)
{
    if (!_dbd)
        return -1;

    if (!table || !columns || !value)
        return -1;

    if (!isQuerying())
        return -1;

    const DBTable* dbt = _dbd->getTable(table);
    if (!dbt)
        return -1;

    const DBColumn* dbc = dbt->getColumns();
    if (!dbc)
        return -1;

    size_t cnum = dbt->getColumnNum();
    if (!cnum)
        return -1;

    StringBuffer sql;
    sql << "UPDATE `" << dbt->getName() << "` SET ";

    Tokenizer tk(columns, ',');
    size_t size = tk.size();
    if (!size)
        return -1;

    char* pval = const_cast<char*>(value);
    const DBColumn* pcol = 0;
    for (size_t i = 0; i < size; ++i)
    {
        pcol = dbt->hasColumn(tk[i].c_str());
        if (pcol)
        {
            sql << "`" << pcol->name << "` = ";
            switch (pcol->type)
            {
                case DB_TINY:
                    sql << *reinterpret_cast<unsigned char*>(pval);
                    pval += sizeof(unsigned char);
                    break;
                case DB_SHORT:
                    sql << *reinterpret_cast<unsigned short*>(pval);
                    pval += sizeof(unsigned short);
                    break;
                case DB_LONG:
                    sql << *reinterpret_cast<unsigned int*>(pval);
                    pval += sizeof(unsigned int);
                    break;
                case DB_LONGLONG:
                    sql << *reinterpret_cast<unsigned long long*>(pval);
                    pval += sizeof(unsigned long long);
                    break;
                case DB_FLOAT:
                    sql << *reinterpret_cast<float*>(pval);
                    pval += sizeof(float);
                    break;
                case DB_DOUBLE:
                    sql << *reinterpret_cast<double*>(pval);
                    pval += sizeof(double);
                    break;
                case DB_VARCHAR:
                    break;
                case DB_STRING:
                    if (true)
                    {
                        size_t len = strlen(pval); // XXX: strlen???
                        Buffer b(len*2+1);
                        unsigned long ret = mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()), pval, len);
                        sql << "\'";
                        if (ret > pcol->size)
                        {
                            ret = pcol->size;
                            sql.put(reinterpret_cast<const char*>(b.rbuf()), ret);
                        }
                        else 
                        {
                            sql << b.rbuf();
                        }

                        sql << "\'";
                        pval += pcol->size;
                        // XXX: 从设计角度考虑，在设计时考虑使用结构体存放字符串
                        // 建议使用CHAR(X)
                    }
                    break;
                case DB_BLOBZ: // XXX: copied from Insert
                    if (true)
                    {
                        dbbinlen_t len = *reinterpret_cast<dbbinlen_t*>(pval);
                        pval += sizeof(len);
                        if (DB_ZIPLEN(len))
                        {
                            if (DB_ZIPFLAG(len))
                            {
                                Zipper zipper(pval, DB_ZIPLEN(len));
                                Buffer b(((sizeof(len)+zipper.size())<<1)+1);
                                unsigned long ret = mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()),
                                        reinterpret_cast<const char*>(&len), sizeof(len));
                                b.wflip(ret);
                                mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()),
                                        reinterpret_cast<const char*>(&zipper), zipper.size());
                                sql << "\'" << b.rbuf() << "\'";
                            }
                            else
                            {
                                Buffer b(((sizeof(len)+len)<<1)+1);
                                unsigned long ret = mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()),
                                        reinterpret_cast<const char*>(&len), sizeof(len));
                                b.wflip(ret);
                                if (len)
                                    mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()), pval, len);
                                sql << "\'" << b.rbuf() << "\'";
                            }
                        }
                        else
                        {
                            char b[(sizeof(DB_ZIPLEN(len))<<1)+1] = {0};
                            mysql_real_escape_string(&_mysql, &b[0],
                                    reinterpret_cast<const char*>(&len), sizeof(len));
                            sql << "\'" << b << "\'";
                        }
                        pval += DB_ZIPLEN(len);
                    }
                    break;
                case DB_X:
                    break;
                default:
                    break;
            }

            if (i < size - 1)
                sql << ",";
        }
    }

    if (where)
        sql << " WHERE " << where;
    int ret = query(&sql, sql.rsize());
    if (!ret)
    {
        unsigned long ret = mysql_affected_rows(&_mysql);
        if (ret && ret != static_cast<unsigned long>(-1))
            return ret;
    }
    return -1;
}

int DBMysqlConnection::sqlInsert(
    const char* table,
        const char* columns,
            const char* value,
                int rows)
{
    return sqlInsertOrReplace(table, columns, value, rows, false);
}

int DBMysqlConnection::sqlReplace(
    const char* table,
        const char* columns,
            const char* value,
                int rows)
{
    return sqlInsertOrReplace(table, columns, value, rows, true);
}

int DBMysqlConnection::sqlInsertOrReplace(
    const char* table,
        const char* columns,
            const char* value,
                int rows,
                    bool replace)
{
    if (!_dbd)
        return -1;

    if (!table || !columns || !value || !rows)
        return -1;

    if (!isQuerying())
        return -1;

    const DBTable* dbt = _dbd->getTable(table);
    if (!dbt)
        return -1;

    const DBColumn* dbc = dbt->getColumns();
    if (!dbc)
        return -1;

    size_t cnum = dbt->getColumnNum();
    if (!cnum)
        return -1;

    StringBuffer sql;
    if (replace)
        sql << "REPLACE INTO `";
    else
        sql << "INSERT INTO `";
    sql << dbt->getName() << "` ";

    std::vector<DBColumn> dbcs;
    const DBColumn* pdbc = dbc;
    size_t ncnum = cnum;
    if (!strchr(columns, '*'))
    {
        Tokenizer tk(columns, ',');
        size_t size = tk.size();
        if (!size)
            return -1;

        sql << "(";
        const DBColumn* pcol = 0;
        for (size_t i = 0; i < size; ++i)
        {
            pcol = dbt->hasColumn(tk[i].c_str());
            if (pcol)
            {
                if (i && i < size)
                    sql << ",`";
                else
                    sql << "`";
                sql << tk[i].c_str() << "`";
                dbcs.push_back(*pcol);
            }
        }
        sql << ")";

        pdbc = &dbcs[0];
        ncnum = dbcs.size();
    }
    sql << " VALUES ";

    int nrows = rows;
    char* pval = const_cast<char*>(value);
    while (nrows)
    {
        sql << "(";

        for (size_t i = 0; i < ncnum; ++i)
        {
            switch (pdbc[i].type)
            {
                case DB_TINY:
                    sql << *reinterpret_cast<unsigned char*>(pval);
                    pval += sizeof(unsigned char);
                    break;
                case DB_SHORT:
                    sql << *reinterpret_cast<unsigned short*>(pval);
                    pval += sizeof(unsigned short);
                    break;
                case DB_LONG:
                    sql << *reinterpret_cast<unsigned long*>(pval);
                    pval += sizeof(unsigned long);
                    break;
                case DB_LONGLONG:
                    sql << *reinterpret_cast<unsigned long long*>(pval);
                    pval += sizeof(unsigned long long);
                    break;
                case DB_FLOAT:
                    sql << *reinterpret_cast<float*>(pval);
                    pval += sizeof(float);
                    break;
                case DB_DOUBLE:
                    sql << *reinterpret_cast<double*>(pval);
                    pval += sizeof(double);
                    break;
                case DB_VARCHAR:
                    break;
                case DB_STRING:
                    if (true)
                    {
                        size_t len = strlen(pval); // XXX: strlen???
                        Buffer b(len*2+1);
                        unsigned long ret = mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()), pval, len);
                        sql << "\'";
                        if (ret > pdbc[i].size)
                        {
                            ret = pdbc[i].size;
                            sql.put(reinterpret_cast<const char*>(b.rbuf()), ret);
                        }
                        else 
                        {
                            sql << b.rbuf();
                        }
                        sql << "\'";
                        pval += pdbc[i].size;
                    }
                    break;
                case DB_BLOBZ:
                    if (true)
                    {
                        dbbinlen_t len = *reinterpret_cast<dbbinlen_t*>(pval);
                        pval += sizeof(len);
                        if (DB_ZIPLEN(len))
                        {
                            if (DB_ZIPFLAG(len))
                            {
                                Zipper zipper(pval, DB_ZIPLEN(len));
                                Buffer b(((sizeof(len)+zipper.size())<<1)+1);
                                unsigned long ret = mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()),
                                        reinterpret_cast<const char*>(&len), sizeof(len));
                                b.wflip(ret);
                                mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()),
                                        reinterpret_cast<const char*>(&zipper), zipper.size());
                                sql << "\'" << b.rbuf() << "\'";
                            }
                            else
                            {
                                Buffer b(((sizeof(len)+len)<<1)+1);
                                unsigned long ret = mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()),
                                        reinterpret_cast<const char*>(&len), sizeof(len));
                                b.wflip(ret);
                                if (len)
                                    mysql_real_escape_string(&_mysql, reinterpret_cast<char*>(b.wbuf()), pval, len);
                                sql << "\'" << b.rbuf() << "\'";
                            }
                        }
                        else
                        {
                            char b[(sizeof(DB_ZIPLEN(len))<<1)+1] = {0};
                            mysql_real_escape_string(&_mysql, &b[0],
                                    reinterpret_cast<const char*>(&len), sizeof(len));
                            sql << "\'" << b << "\'";
                        }
                        pval += DB_ZIPLEN(len);
                    }
                    break;
                case DB_X:
                    break;
                default:
                    break;
            }
            if (i < ncnum-1)
                sql << ",";
        }

        --nrows;
        if (nrows)
            sql << "),";
        else
            sql << ");";
    }

    int ret = query(&sql, sql.rsize());
    if (!ret)
    {
        unsigned long ret = mysql_affected_rows(&_mysql);
        if (ret && ret != static_cast<unsigned long>(-1))
            return rows;
        if (!ret)
            return ret;
    }
    return -1;
}

int DBMysqlConnection::sqlDelete(const char* table, const char* where)
{
    if (!table)
        return -1;
    int ret = -1;
    if (where)
        ret = vquery("DELETE FROM %s WHERE %s", table, where);
    else
        ret = vquery("DELETE FROM %s", table);
    if (!ret)
        return mysql_affected_rows(&_mysql); // XXX: my_ulongulong
    return -1;
}

int DBMysqlConnection::count(const char* table, const char* where)
{
    if (!table)
        return -1;
    int ret = -1;
    if (where)
        ret = vquery("SELECT COUNT(*) FROM %s WHERE %s", table, where);
    else
        ret = vquery("SELECT COUNT(*) FROM %s", table);
    if (ret < 0)
        return ret;

    MYSQL_RES* res = mysql_store_result(&_mysql);
    if (res)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row)
        {
            if (row[0])
            {
                return atoi(row[0]);
            }
        }
        mysql_free_result(res);
    }

    if (mysql_errno(&_mysql) == CR_SERVER_LOST ||
            mysql_errno(&_mysql) == CR_UNKNOWN_ERROR)
        setState(DBCS_LOST);
    return -1;
}

int DBMysqlConnection::commit()
{
    return vquery("COMMIT;");
}

int DBMysqlConnection::rollback()
{
    return vquery("ROLLBACK;");
}

#define STMT_ERROR_TEST(n)          \
    mysql_stmt_close(_stmt);        \
    _stmt = 0;                      \
    _meta = 0;                      \
switch(n)                           \
{                                   \
    case CR_SOCKET_CREATE_ERROR:    \
    case CR_CONNECTION_ERROR:       \
    case CR_CONN_HOST_ERROR:        \
    case CR_IPSOCK_ERROR:           \
    case CR_SERVER_GONE_ERROR:      \
    case CR_SERVER_LOST:            \
    case CR_TCP_CONNECTION:         \
        setState(DBCS_LOST);        \
        return -1;                  \
    default:                        \
         return -1;                 \
}

// XXX: must before call bind()
bool DBMysqlConnection::checkSize()
{
    if (!_meta)
        return false;

    for (size_t i = 0; i < _length.size(); ++i)
    {
        DBOMeta& m = (*_meta)[i];
        unsigned long length = _length[i];

        if (m.type == DBOT_BASE)
        {
            if (m.size < length)
                return false;
        }
        else if (m.type == DBOT_STR)
        {
            reinterpret_cast<std::string*>(m.data)->resize(length, 0);
            m.size = length;
        }
        else if (m.type == DBOT_VEC)
        {
            reinterpret_cast<std::vector<unsigned char>*>(m.data)->resize(length, 0);
            m.size = length;
        }
        else if (m.type == DBOT_BUFFER)
        {
            reinterpret_cast<Buffer*>(m.data)->resize(length);
            m.size = length;
        }
        else
        {
            return false;
        }
    }
    return true;
}

inline size_t fieldSize(const MYSQL_FIELD& field)
{
    switch (field.type)
    {
        case MYSQL_TYPE_TINY:     return sizeof(u8_t);
        case MYSQL_TYPE_SHORT:    return sizeof(u16_t);
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:     return sizeof(u32_t);
        case MYSQL_TYPE_FLOAT:    return sizeof(f32_t);
        case MYSQL_TYPE_DOUBLE:   return sizeof(f64_t);
        case MYSQL_TYPE_LONGLONG: return sizeof(u64_t);

        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_BIT:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_SET:
        case MYSQL_TYPE_GEOMETRY:
        default:
            return field.length;
    }
    return field.length;
}

bool DBMysqlConnection::fixSize()
{
    if (!_meta)
        return false;

    _orglength = _length;

    bool fixed = false;
    for (size_t i = 0; i < _length.size(); ++i)
    {
        DBOMeta& m = (*_meta)[i];
        unsigned long length = _orglength[i];

        if (m.type == DBOT_STR)
        {
            if (length > m.size)
            {
                m.size = length;
                reinterpret_cast<std::string*>(m.data)->resize(m.size, 0);
                _bind[i].buffer = &(*reinterpret_cast<std::string*>(m.data))[0];
                _bind[i].buffer_length = m.size;
                fixed = true;
            }
        }
        else if (m.type == DBOT_VEC)
        {
            if (length > m.size)
            {
                m.size = length;
                reinterpret_cast<std::vector<unsigned char>*>(m.data)->resize(m.size, 0);
                _bind[i].buffer = &(*reinterpret_cast<std::vector<unsigned char>*>(m.data))[0];
                _bind[i].buffer_length = m.size;
                fixed = true;
            }
        }
        else if (m.type == DBOT_BUFFER)
        {
            if (length > m.size)
            {
                m.size = length;
                reinterpret_cast<Buffer*>(m.data)->resize(m.size);
                _bind[i].buffer = &(*reinterpret_cast<Buffer*>(m.data))[0];
                _bind[i].buffer_length = m.size;
                fixed = true;
            }
        }
    }

    if (fixed)
    {
        if (mysql_stmt_bind_result(_stmt, &_bind[0]))
            return false;
    }

    return true;
}

bool DBMysqlConnection::rebindField()
{
    if (!_meta)
        return false;

    bool rebind = false;
    for (size_t i = 0; i < _orglength.size(); ++i)
    {
        DBOMeta& m = (*_meta)[i];
        unsigned long length = _orglength[i];
        m.isnull = 0;

        if (m.type == DBOT_STR)
        {
            if (length > reinterpret_cast<std::string*>(m.data)->size())
            {
                m.size = length;
                reinterpret_cast<std::string*>(m.data)->resize(m.size, 0);
                _bind[i].buffer = &(*reinterpret_cast<std::string*>(m.data))[0];
                _bind[i].buffer_length = m.size;
                rebind = true;
            }
        }
        else if (m.type == DBOT_VEC)
        {
            if (length > reinterpret_cast<std::vector<unsigned char>*>(m.data)->size())
            {
                m.size = length;
                reinterpret_cast<std::vector<unsigned char>*>(m.data)->resize(m.size, 0);
                _bind[i].buffer = &(*reinterpret_cast<std::vector<unsigned char>*>(m.data))[0];
                _bind[i].buffer_length = m.size;
                rebind = true;
            }
        }
        else if (m.type == DBOT_BUFFER)
        {
            // TODO: fillzero
            reinterpret_cast<Buffer*>(m.data)->clear();
            if (length > reinterpret_cast<Buffer*>(m.data)->wsize())
            {
                m.size = length;
                reinterpret_cast<Buffer*>(m.data)->resize(m.size);
                _bind[i].buffer = &(*reinterpret_cast<Buffer*>(m.data))[0];
                _bind[i].buffer_length = m.size;
                rebind = true;
            }
        }
    }

    if (rebind)
    {
        if (mysql_stmt_bind_result(_stmt, &_bind[0]))
            return false;
    }

    return true;
}

bool DBMysqlConnection::bind(MYSQL_FIELD* field, int count)
{
    if (!_meta || !field || !count)
        return false;

    for (int i = 0; i < count; ++i)
    {
        DBOMeta& m = (*_meta)[i];

        _bind[i].buffer_type = field[i].type;
        _bind[i].buffer_length = m.size;
        if (m.type == DBOT_BASE)
            _bind[i].buffer = m.data;
        else if (m.type == DBOT_STR)
            _bind[i].buffer = &(*reinterpret_cast<std::string*>(m.data))[0];
        else if (m.type == DBOT_VEC)
            _bind[i].buffer = &(*reinterpret_cast<std::vector<unsigned char>*>(m.data))[0];
        else if (m.type == DBOT_BUFFER)
            _bind[i].buffer = reinterpret_cast<Buffer*>(m.data)->wbuf();
        _bind[i].length = &_length[i];
        _bind[i].is_null = reinterpret_cast<my_bool*>(&m.isnull);
        _bind[i].is_unsigned = (my_bool) ((field[i].flags & UNSIGNED_FLAG) == UNSIGNED_FLAG);
        m.isunsigned = _bind[i].is_unsigned;
    }

    if (mysql_stmt_bind_result(_stmt, &_bind[0]))
        return false;
    return true;
}

int DBMysqlConnection::prepare(const char* sql, std::vector<DBOMeta>& meta)
{
    if (!sql)
        return -1;
    if (!isConnected())
        return -1;
    if (isQuerying())
        return -1;
    size_t len = strlen(sql);
    if (!len)
        return -1;

    _meta = &meta;
    _stmt = mysql_stmt_init(&_mysql);
    if (!_stmt)
        return -1;

    int count = 0;
    MYSQL_RES* res = 0;
    MYSQL_FIELD* fields = 0;

    if(mysql_stmt_prepare(_stmt, sql, len))
        goto _error;

    res = mysql_stmt_result_metadata(_stmt);
    if (!res)
        goto _error;

    count = mysql_num_fields(res);
    if (count != static_cast<int>(meta.size()))
        goto _error;

    if (mysql_stmt_execute(_stmt))
        goto _error;

    //  mysql_stmt_bind_param()

    if (count)
    {
        _bind.resize(count);
        _length.resize(count, 0);
        MEMZRO(&_bind[0], count*sizeof(MYSQL_BIND));
    }

    fields = mysql_fetch_fields(res);
    if (!fields)
        goto _error;

    for (int i = 0; i < count; ++i)
    {
        _length[i] = fieldSize(fields[i]);
    }

    if (!checkSize())
        goto _error;

    if (!bind(fields, count))
        goto _error;

    if (!fixSize())
        goto _error;

    setState(DBCS_PREPARING|DBCS_CONNECTED);
    return 0;

_error:
    STMT_ERROR_TEST(mysql_stmt_errno(_stmt));
}

void DBMysqlConnection::checkNull(size_t off)
{
    if (!_meta)
        return;
    if (off >= _meta->size())
        return;

    DBOMeta& m = (*_meta)[off];
    if (m.type == DBOT_BASE && *_bind[off].is_null)
    {
        if (m.size == 1)
            *reinterpret_cast<u8_t*>(m.data) = 0;
        else if (m.size == 2)
            *reinterpret_cast<u16_t*>(m.data) = 0;
        else if (m.size == 4)
            *reinterpret_cast<u32_t*>(m.data) = 0;
        else if (m.size == 8)
            *reinterpret_cast<u64_t*>(m.data) = 0;
    }
    else if (m.type == DBOT_STR)
    {
        if (*_bind[off].is_null)
            reinterpret_cast<std::string*>(m.data)->clear();
        else
            reinterpret_cast<std::string*>(m.data)->resize(_length[off]);
    }
    else if (m.type == DBOT_VEC)
    {
        if (*_bind[off].is_null)
            reinterpret_cast<std::vector<unsigned char>*>(m.data)->clear();
        else
            reinterpret_cast<std::vector<unsigned char>*>(m.data)->resize(_length[off]);
    }
    else if (m.type == DBOT_BUFFER)
    {
        if (*_bind[off].is_null)
            reinterpret_cast<Buffer*>(m.data)->clear(); // reinterpret_cast<Buffer*>(m.data)->rflip(reinterpret_cast<Buffer*>(m.data)->rsize());
        else
            reinterpret_cast<Buffer*>(m.data)->wflip(*_bind[off].length);
    }
}

int DBMysqlConnection::next()
{
    if (!rebindField())
    {
        finish();
        return -1;
    }

    int r = mysql_stmt_fetch(_stmt);
    switch (r)
    {
        case 0:
        {
            size_t sz = _bind.size();
            for (size_t i = 0; i < sz; ++i)
            {
                mysql_stmt_fetch_column(_stmt, &_bind[i], i, 0);
                checkNull(i);
            }
            return 0;
        }
        break;

        case MYSQL_NO_DATA:
        {
            finish();
            return -1;
        }
        break;

        default:
        break;
    }

    finish();
    return -1;
}

void DBMysqlConnection::finish()
{
    _bind.clear();
    _length.clear();
    _orglength.clear();

    if (_stmt)
    {
        mysql_stmt_close(_stmt);
        _stmt = 0;
    }
    if (_meta)
        _meta = 0;
    if (isPreparing())
        setState(DBCS_CONNECTED);
}

unsigned long DBMysqlConnection::escape(char* to, const char* from, size_t length)
{
    if (!from || !to || !length)
        return 0;
    if (!isConnected())
        return 0;

    return mysql_real_escape_string(&_mysql, to, from, length);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

