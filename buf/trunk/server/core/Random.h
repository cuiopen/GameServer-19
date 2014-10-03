
#ifndef RANDOM_H_
#define RANDOM_H_

namespace buf
{

struct Random
{
    int operator()();
    int operator()(int max);
    int operator()(int min, int max);
};

} // namespace buf

#endif // RANDOM_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

