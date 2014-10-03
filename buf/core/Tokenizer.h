
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "Spliter.h"
#include <string>
#include <vector>

namespace buf
{

class Tokenizer
{
public:
    typedef std::string value_type;
    typedef std::vector<value_type> container;
    typedef container::size_type size_type;
    typedef container::iterator iterator;

public:
    Tokenizer(const char* tokens, char sp = ' ')
    {
        if (tokens)
            split(_toks, tokens, sp);
    }

    Tokenizer(const char* tokens, const char* sp = " ")
    {
        if (tokens && sp)
            split(_toks, tokens, sp);
    }

    Tokenizer(const std::string& tokens, char sp = ' ')
    {
        if (tokens.length())
            split(_toks, tokens, sp);
    }

    Tokenizer(const std::string& tokens, const std::string& sp = " ")
    {
        if (tokens.length() && sp.length())
            split(_toks, tokens, sp);
    }

    Tokenizer(const Tokenizer& tk) : _toks(tk._toks) {}
    ~Tokenizer() { clear(); }

    inline size_type size() const { return _toks.size(); }
    inline void clear() { _toks.clear(); }

    inline Tokenizer& operator=(const Tokenizer& tk)
    {
        _toks = tk._toks;
        return *this;
    }

    inline const value_type& operator[](size_type i) const
    {
        if (i < _toks.size())
            return _toks[i];

        static std::string null = "";
        return null;
    }

    inline value_type& operator[](size_type i)
    {
        if (i < _toks.size())
            return _toks[i];

        static std::string null = "";
        return null;
    }

    inline void pop_front()
    {
        if (_toks.size())
            _toks.erase(_toks.begin());
    }

    inline iterator begin() { return _toks.begin(); }
    inline iterator end() { return _toks.end(); }

    inline void pop_back() { _toks.pop_back(); }

private:
    container _toks;
};

} // namespace buf

#endif // TOKENIZER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

