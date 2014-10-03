
#ifndef INI_H_
#define INI_H_

#include "Config.h"
#include "Encoding.h"
#include "UnorderedContainer.h"
//#include <stdlib.h> // for size_t
#include <string>

namespace buf
{

const unsigned int INI_LINESIZE = 4096;

// TODO: support [group]
class INI
{
public:
    typedef UNORDERED_MAP<std::string, std::string> group;
    typedef UNORDERED_MAP<std::string, group> container;
    typedef group::iterator giterator;
    typedef group::const_iterator const_giterator;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

    typedef container::value_type::first_type first_type;
    typedef container::value_type::second_type second_type;

public:
    INI() {}
    ~INI() {}

    bool open(const char* path);
    bool readMemory(const char* buffer, size_t size);

    bool newINI();

    bool newGroup(const char* group);
    bool newNode(const char* key, const char* value, const char* group);
    const char* getValue(const char* key, const char* group = 0);

    bool saveas(const char* path);

    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }

private:
    container _data;
    std::string _curgrp;
};

} // namespace buf

#endif // INI_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

