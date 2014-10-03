
#ifndef DBDESCRIPTION_H_
#define DBDESCRIPTION_H_

#include "Config.h"

#include <string>
#include <map>
#include <vector>

namespace buf
{

#define SIZEOF_TINYINT      1
#define SIZEOF_SMALLINT     2
#define SIZEOF_INT          4
#define SIZEOF_MEDIUMINT    4 // 3
#define SIZEOF_BIGINT       8
#define SIZEOF_FLOAT        4
#define SIZEOF_DOUBLE       8
#define SIZEOF_DATE         4 // 3
#define SIZEOF_DATETIME     8
#define SIZEOF_TIMESTAMP    4
#define SIZEOF_TIME         4 // 3
#define SIZEOF_YEAR         1

enum DBColumnType
{
    DB_TINY = 1,        // 1 bytes
    DB_SHORT,           // 2 bytes
    DB_INT,             // 4 bytes
    DB_LONG = DB_INT,   // 4 bytes
    DB_LONGLONG,        // 8 bytes
    DB_FLOAT,           // 4 float
    DB_DOUBLE,          // 8 double
    DB_VARCHAR,         // varchar TODO: not implemented
                        // XXX: the value must be terminated with a '\0'
    DB_STRING,          // char, varchar
                        // XXX: the value must be terminated with a '\0'
                        // XXX: the length of value must be equal to the field length
    DB_BLOBZ,           // blob or text and may be zipped
                        // XXX: DB_ZIPSET(len) will set zip flag for the data
    DB_X,               // my format

    // aliases
    DB_BYTE = DB_TINY,
    DB_WORD = DB_SHORT,
    DB_DWORD = DB_INT,
    DB_QWORD = DB_LONGLONG,

    DB_UNKNOW
};

struct DBColumn
{
    DBColumn() : type(0), size(0), name("") {}
    DBColumn(const DBColumn& col)
        : type(col.type), size(col.size), name(col.name) {}
    DBColumn& operator=(const DBColumn& col)
    {
        type = col.type;
        size = col.size;
        name = col.name;
        return *this;
    }

    short type;
    unsigned int size;
    std::string name;
};

inline bool isTiny(const DBColumn& col) { return col.type == DB_TINY; }
inline bool isShort(const DBColumn& col) { return col.type == DB_SHORT; }
inline bool isLong(const DBColumn& col) { return col.type == DB_LONG; }
inline bool isLongLong(const DBColumn& col) { return col.type == DB_LONGLONG; }
inline bool isFloat(const DBColumn& col) { return col.type == DB_FLOAT; }
inline bool isDouble(const DBColumn& col) { return col.type == DB_DOUBLE; }
inline bool isString(const DBColumn& col) { return col.type == DB_STRING; }
inline bool isBlobz(const DBColumn& col) { return col.type == DB_BLOBZ; }
inline bool isX(const DBColumn& col) { return col.type == DB_X; }

inline bool isByte(const DBColumn& col) { return col.type == DB_BYTE; }
inline bool isWord(const DBColumn& col) { return col.type == DB_WORD; }
inline bool isDword(const DBColumn& col) { return col.type == DB_DWORD; }
inline bool isQword(const DBColumn& col) { return col.type == DB_QWORD; }

class DBTable
{
public:
    DBTable(const char* name, int ncol);
    DBTable(const std::string& name, int ncol);
    DBTable(const DBTable& table);
    ~DBTable();

    const DBTable& operator=(const DBTable& table);
    void addColumn(const DBColumn& col);

    inline const std::string& getName() const { return _name; }
    size_t getColumnNum() const { return _columns.size(); }
    const DBColumn* getColumns() const { return &_columns[0]; }
    const DBColumn* hasColumn(const char* col) const;

private:
    std::string _name;
    std::vector<DBColumn> _columns;
};

class DBDescription
{
public:
    typedef std::map<std::string, const DBTable*> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

public:
    DBDescription();
    ~DBDescription();

    void addTable(const DBTable* table);
    const DBTable* getTable(const std::string& tablename) const;

private:
    container _tables;
};

} // namespace buf

#endif // DBDESCRIPTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

