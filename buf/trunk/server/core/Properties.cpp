
#include "Properties.h"

namespace buf
{

static std::string null_string = "";

std::string& Properties::operator[](const std::string& key)
{
    iterator i = _props.find(key);
    if (i == _props.end())
    {
        std::pair<iterator, bool> ret = _props.insert(std::make_pair(key, ""));
        if (ret.second)
        {
            ++_count;
            return ret.first->second;
        } else
            return null_string;
    }
    return i->second;
}

const std::string& Properties::operator[](const std::string& key) const
{
    const_iterator i = _props.find(key);
    if (i == _props.end())
        return null_string;
    else
        return i->second;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

