
#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

namespace buf
{

typedef size_t size_type;

struct string_equ
{
    bool operator()(const std::string& s1, const std::string& s2) const
    {
        return s1 == s2;
    }
};

struct string_hash
{
    size_type operator()(const std::string& str) const
    {
        return __gnu_cxx::hash<const char*>()(str.c_str());
    }
};

class Properties
{
public:
#ifdef __GNUC__
    typedef __gnu_cxx::hash_map<std::string, std::string, string_hash, string_equ> Container;
#else
    typedef std::map<std::string, std::string> Container;
#endif

    typedef Container::iterator iterator;
    typedef Container::const_iterator const_iterator;

public:
    Properties() : _count(0) {}
    ~Properties() { clear(); }
    
    std::string& operator[](const std::string& key);
    const std::string& operator[](const std::string& key) const;

    size_type count() const { return _count; }
    void clear() { _props.clear(); }

private:
    Container _props;
    size_type _count;
};

} // namespace buf

#endif // PROPERTIES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

