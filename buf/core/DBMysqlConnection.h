
#ifndef DBMYSQLCONNECTION_H_
#define DBMYSQLCONNECTION_H_

#include "DBConnection.h"
#include "Buffer.h"
#include <mysql.h>

namespace buf
{

class DBMysqlConnection : public DBConnection
{
public:
    static DBDescription* createDescription(const Url& url);

public:
    DBMysqlConnection(DBDescription* dbd);
    virtual ~DBMysqlConnection();

    inline bool connect(const Url& url)
    {
        return DBConnection::connect(url);
    }

    virtual bool connect(
        const char* host,
            const char* user,
                const char* passwd,
                    const char* db,
                        unsigned short port);

    virtual void close();

    virtual bool setCharset(const char* charset = DB_CHAR_SET_UTF8);

    // XXX: only for query which do not need result
    virtual int vquery(const char* sql, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 2, 3)));
#else
        ;
#endif

    virtual int vquery(Buffer& result, const char* sql, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 3, 4)));
#else
        ;
#endif

    virtual int query(const char* sql, int length, bool dropret = false);
    virtual int query(Buffer& result, const char* sql, int length);

    // 同一时间一个Connection只能有一个sql语句被执行
    // select [column].* from db.table [where stmt group by stmt order by stmt]
    virtual int sqlSelect(
        const char* table,
            const char* columns,
                char* result, /* XXX: you must provide enought memory */
                    int from = 0,
                        int limit = 1,
                            const char* where = 0,
                                const char* order = 0,
                                    const char* group = 0);

    // XXX: 返回值为0时也可能是空返回集，可以根据result.rsize()来决定是否有数据
    virtual int sqlSelect(
        const char* table,
            const char* columns,
                Buffer& result,
                    int from = 0,
                        int limit = 1,
                            const char* where = 0,
                                const char* order = 0,
                                    const char* group = 0);

    virtual int sqlUpdate(
        const char* table,
            const char* columns,
                const char* value,
                    const char* where = 0);

    virtual int sqlInsert(
        const char* table,
            const char* columns,
                const char* value,
                    int rows = 1);

    virtual int sqlReplace(
        const char* table,
            const char* columns,
                const char* value,
                    int rows = 1);

    virtual int sqlDelete(const char* table, const char* where);

    virtual int count(const char* table, const char* where = NULL);
    virtual int commit();
    virtual int rollback();

    // XXX: 基于stmt读取
    virtual int prepare(const char* sql, std::vector<DBOMeta>& meta);
    virtual int next();
    virtual void finish();

    virtual unsigned long escape(char* to, const char* from, size_t length);

protected:
    int sqlInsertOrReplace(
        const char* table,
            const char* columns,
                const char* value,
                    int rows,
                        bool replace = false);

    int fetchRows(Buffer& buffer, size_t ncnum = -1);

private:
    bool checkSize();
    bool bind(MYSQL_FIELD* field, int count);
    bool fixSize();
    bool rebindField();
    void checkNull(size_t off);

private:
    MYSQL _mysql;

    // XXX: for preparing
    MYSQL_STMT* _stmt;
    std::vector<DBOMeta>* _meta;
    std::vector<MYSQL_BIND> _bind;
    std::vector<unsigned long> _length;
    std::vector<unsigned long> _orglength;
};

} // namespace buf

#endif // DBMYSQLCONNECTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

