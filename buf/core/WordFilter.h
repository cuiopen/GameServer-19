
#ifndef WORDFILTER_H_
#define WORDFILTER_H_

#include "Singleton.h"
#include <string>
#include <map>

namespace buf
{

class WordFilter : public Singleton<WordFilter>
{
public:
    typedef std::map<std::string, std::string> container;
    typedef container::iterator iterator;

public:
    WordFilter() {}
    ~WordFilter() {}

    bool init(const char* file);
    bool filter(char* text, size_t len, bool firstbreak = false);

private:
    container _filterWords;
};

} // namespace buf

#endif // WORDFILTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

