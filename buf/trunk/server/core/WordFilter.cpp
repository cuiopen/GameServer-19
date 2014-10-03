
#include "WordFilter.h"
#include "Tokenizer.h"
#include <fstream>

#ifndef CHAT_MAX_LEN
#define CHAT_MAX_LEN 1024
#endif

namespace buf
{

bool WordFilter::init(const char* file)
{
    if (!file)
        return false;

    std::ifstream src(file);
    if (!src)
        return false;

    char buf[1024] = {0};
    Tokenizer::iterator word;
    Tokenizer::iterator replacer;
    while (src.getline(buf, sizeof(buf)))
    {
        Tokenizer tk(buf, ' ');
        if (tk.size() == 2)
        {
            replacer = tk.begin();
            word = replacer++;

            MEMZRO(buf,sizeof(buf));
            word->copy(buf, word->length(), 0);
            for (unsigned int i = 0; i < word->length() && i < (sizeof(buf)-1); ++i)
                buf[i] = tolower(buf[i]);
            word->assign(buf);

            _filterWords[*word] = *replacer;
        }
    }
    return true;
}

bool WordFilter::filter(char* text, size_t len, bool firstbreak)
{
    if (!text || !len)
        return false;

    bool ret = false;
    char copy[CHAT_MAX_LEN+1] = {0};
    strncpy(copy, text, CHAT_MAX_LEN);
    size_t slen = strlen(copy);
    for (unsigned int i = 0; i < slen && i <(sizeof(copy)-1); ++i)
        copy[i] = tolower(copy[i]);

    std::string content(text);
    std::string content_copy(copy);
    std::string::size_type pos = 0;

    for (iterator it = _filterWords.begin(); it != _filterWords.end(); ++it)
    {
        pos = content_copy.find(it->first.c_str(), 0);
        while (pos != std::string::npos)
        {
            content.replace(pos, it->first.length(), it->second.c_str());
            content_copy.replace(pos, it->first.length(), it->second.c_str());
            pos = content_copy.find(it->first.c_str(), pos+it->first.length());
            ret = true;
        }
        if (firstbreak)
            break;
    }

    strncpy(text, content.c_str(), len);
    return ret;
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

