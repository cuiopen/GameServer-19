
#ifndef BITVEC_H_
#define BITVEC_H_

#include "Config.h"
#include <vector>

namespace buf
{

// XXX: idx start from 0

#define POS(x) (1<<((x)&0x7))

class BitVec
{
public:
    BitVec() : _bits(32) {}
    BitVec(u16_t bits) : _bits(bits/8, 0) {}
    ~BitVec() {}

    // 7 <---- 0
    // 15<---- 8

    inline bool get(u16_t bit)
    {
        if (!checksize(bit))
            return false;
        return _bits[bit>>3] & POS(bit);
    }

    inline void set(u16_t bit, bool val = true)
    {
        if (!checksize(bit))
            return;
        if (val)
            _bits[bit>>3] |= POS(bit);
        else
            _bits[bit>>3] &= ~POS(bit);
    }

    inline void clear(u16_t bit)
    {
        if (!checksize(bit))
            return;
        _bits[bit>>3] &= ~POS(bit);
    }

    inline void reset()
    {
        MEMZRO(&_bits, sizeof(_bits));
    }

    inline size_t blockSize() const { return _bits.size(); }
    inline const u8_t* getBlock() const { return &_bits[0]; }

private:
    bool checksize(u16_t bit)
    {
        if ((bit>>3) >= _bits.size())
            _bits.resize(_bits.size()+32);
        return true;
    }

protected:
    std::vector<u8_t> _bits;
};

#undef POS

} // namespace buf

#endif // BITVEC_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

