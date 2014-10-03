
#ifndef FOREACH_H_
#define FOREACH_H_

#include "Enumerator.h"

namespace buf
{

template <typename I, typename T>
bool foreach(I begin, I end, Enumerator<T>& enumator)
{
    if (begin == end)
        return false;
    for (I i = begin; i != end; ++i)
        if (!enumator(&*i)) return false;
    return true;
}

} // namespace buf

#endif // FOREACH_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

