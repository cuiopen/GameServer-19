
#include "INI.h"
#include "Spliter.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace buf
{

static const char* INI_GLOBAL = "__global__";
    
bool INI::open(const char* path)
{
    if (!path)
        return false;

    std::fstream st;
    st.open(path, std::ios::in);
    if (!st.is_open())
        return false;

    _curgrp = INI_GLOBAL;
    if (!_data.insert(std::make_pair(_curgrp, group())).second)
    {
        st.close();
        return false;
    }

    char buf[INI_LINESIZE+1] = {0};
    do {
        st.getline(buf, sizeof(buf));
        readMemory(buf, sizeof(buf));
    } while (!st.eof());

    st.close();
    return true;
}

bool INI::readMemory(const char* buffer, size_t size)
{
    if (!buffer || !size)
        return false;

    // # commit
    // -- commit
    // \/\/ commit
    
    char* pbuffer = const_cast<char*>(buffer);
    size_t rded = 0;
    while (isspace(*pbuffer))
    {
        ++rded;
        ++pbuffer;
    }

    bool quote_begin = false;
    bool quote_end = false;
    bool first = true;
    char buf[INI_LINESIZE] = {0};
    char* pbuf = buf;
    while (*pbuffer && rded < size)
    {
        if (*pbuffer == '\'' || *pbuffer == '\"')
        {
            if (quote_end)
                break;
            if (quote_begin)
                quote_end = true;
            quote_begin = true;
            ++pbuffer;
            continue;
        }
        if ((!quote_begin || quote_end) && (*pbuffer == '#' ||
            (*pbuffer == '-' && *(pbuffer+1) == '-') ||
            (*pbuffer == '/' && *(pbuffer+1) == '/')))
        {
            if (first)
                return true;
            else
                break;
        }
        else
        {
            first = false;
            *pbuf++ = *pbuffer++;
            ++rded;
        }
    }

    buf[rded] = '\0';
    if (rded)
    {
        std::vector<std::string> tk;
        split(tk, buf, '=');
        trim(tk[0]);
        trim(tk[1]);
        iterator it = _data.find(_curgrp);
        if (it != _data.end())
            it->second[tk[0]] = tk[1];
    }

    return true;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

