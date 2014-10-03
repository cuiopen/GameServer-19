
#ifndef BITS_H_
#define BITS_H_

#include "Config.h"

namespace buf
{

// XXX: idx start from 0

#define POS(x) (1<<((x)&0x7))

template <u32_t size = 8>
class Bit
{
public:
    Bit() {}
    ~Bit() {}

    // 7 <---- 0
    // 15<---- 8

    inline bool get(u32_t bit)
    {
        if (bit >= size)
            return false;
        return _bits[bit>>3] & POS(bit);
    }

    inline void set(u32_t bit, bool val)
    {
        if (bit >= size)
            return;
        if (val)
            _bits[bit>>3] |= POS(bit);
        else
            clear(bit);
    }

    inline void clear(u32_t bit)
    {
        if (bit >= size)
            return;
        _bits[bit>>3] &= ~POS(bit);
    }

    inline void reset()
    {
        MEMZRO(&_bits, sizeof(_bits));
    }

    inline size_t blockCount() const { return (size+7)>>3; }
    inline const u8_t* getBlock() const { return &_bits[0]; }

protected:
    u8_t _bits[(size+7)>>3];
};

#undef POS

} // namespace buf

#endif // BITS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

