
#include "GameObject_ValueFiled.h"

namespace buf
{

void GameObject::setDwordValue(u16_t idx, u32_t v)
{
    ASSERT(idx < _valueCount);
    _dwordValue[idx] = v;
    _updateMask.set(idx);
}

void GameObject::setQwordValue(u16_t idx, const u64_t& v)
{
    ASSERT(idx < _valueCount);
    *reinterpret_cast<u64_t*>(&_dwordValue[idx]) = v;
    _updateMask.set(idx);
    _updateMask.set(idx+1);
}

void GameObject::setFloatValue(u16_t idx, f32_t v)
{
    ASSERT(idx < _valueCount);
    *reinterpret_cast<f32_t*>(&_dwordValue[idx]) = v;
    _updateMask.set(idx);
}

void GameObject::setWordValue(u16_t idx, u8_t off, u16_t v)
{
    ASSERT(idx < _valueCount && off < 2);
    *(reinterpret_cast<u16_t*>(&_dwordValue[idx])+off) = v;
    _updateMask.set(idx);
}

void GameObject::setByteValue(u16_t idx, u8_t off, u8_t v)
{
    ASSERT(idx < _valueCount && off < 4);
    *(reinterpret_cast<u8_t*>(&_dwordValue[idx])+off) = v;
    _updateMask.set(idx);
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

