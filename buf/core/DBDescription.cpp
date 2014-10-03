
#include "DBDescription.h"

namespace buf
{

DBTable::DBTable(const char* name, int ncol)
    : _name(name?name:"") { _columns.reserve(ncol); }

DBTable::DBTable(const std::string& name, int ncol)
    : _name(name) { _columns.reserve(ncol); }

DBTable::DBTable(const DBTable& table)
    : _name(table._name), _columns(table._columns) {}

DBTable::~DBTable() { _columns.clear(); }

void DBTable::addColumn(const DBColumn& col)
{
    _columns.push_back(col);
}

const DBColumn* DBTable::hasColumn(const char* col) const
{
    if (!col)
        return 0;
    size_t size = _columns.size();
    size_t len = strlen(col);

    DBColumn* pcol = const_cast<DBColumn*>(&_columns[0]);
    for (size_t i = 0; i < size; ++i)
    {
        size_t nlen = pcol->name.length();

        if (nlen != len)
        {
            ++pcol;
            continue;
        }

        if (strncmp(pcol->name.c_str(), col, nlen) == 0)
            return pcol;

        ++pcol;
    }
    return 0;
}

const DBTable& DBTable::operator=(const DBTable& table)
{
    _name = table._name;
    //_columns.clear();
    _columns = table._columns;
    return *this;
}

DBDescription::DBDescription() {}

DBDescription::~DBDescription()
{
    for (iterator i = _tables.begin(); i != _tables.end(); ++i)
    {
        if (i->second)
            DELETE(i->second);
    }
    _tables.clear();
}

void DBDescription::addTable(const DBTable* table)
{
    if (!table)
        return;
    iterator i = _tables.find(table->getName());
    if (i != _tables.end())
    {
        if (i->second)
            DELETE(i->second);
        i->second = table;
    } else
        _tables.insert(std::make_pair(table->getName(), table));
}

const DBTable* DBDescription::getTable(const std::string& tablename) const
{
    const_iterator ci = _tables.find(tablename);
    if (ci != _tables.end())
        return ci->second;
    return 0;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

