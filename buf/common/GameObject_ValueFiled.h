
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "core/Config.h"
#include "core/BitVec.h"
#include "core/Assertion.h"
#include "core/Utils.h"
#include "GridIndex.h"
#include <string>

namespace buf
{

// Base fields
enum
{
    GO_GUID     = 0x0000, // Size: 2, Type: u64_t, Flags: PUBLIC
    GO_TYPEID   = 0x0002, // Size: 1, Type: u32_t, Flags: PUBLIC
    GO_END
};

class GameObject
{
public:
    GameObject(u16_t count)
        : _valueCount(count), _dwordValue(0), _updateMask(count)
    {
        initValues();
    }

    virtual ~GameObject()
    {
        if (_dwordValue)
            DELETEVEC(_dwordValue);
        _dwordValue = 0;
    }

    int getType() const { return _objectType; }
    void setType(int v) { _objectType = v; }

    bool setPos(const Pos& mapWH, const Pos& p)
    {
        PosI grid = (PosI)-1;;
        if (posToGrid(mapWH, p, grid))
        {
            _grid = grid;
            _pos = p;
            return true;
        }
        return false;
    }
    const Pos& getPos() const { return _pos; }
    PosI getGrid() const { return _grid; }
    void setGrid(PosI grid) { _grid = grid; }
    bool isInWorld() const { return false; }
    void setInWorld(bool v) { _inWorld = v; }

    inline u16_t getValueCount() const { return _valueCount; }

    const char* getName() const { return _name; }
    void setName(const char* name) { cpystr(name, _name, sizeof(_name), "null"); }

    inline u32_t getDwordValue(u16_t idx) const
    {
        ASSERT(idx < _valueCount);
        return _dwordValue[idx];
    }

    inline u64_t getQwordValue(u16_t idx) const
    {
        ASSERT(idx + 1 < _valueCount);
        return static_cast<u64_t>(_dwordValue[idx]);
    }

    inline f32_t getFloatValue(u16_t idx) const
    {
        ASSERT(idx < _valueCount);
        return static_cast<f32_t>(_dwordValue[idx]);
    }

    inline u16_t getWordValue(u16_t idx, u8_t off) const
    {
        ASSERT(idx < _valueCount && off < 2);
        return *(reinterpret_cast<u16_t*>(&_dwordValue[idx])+off);
    }

    inline u8_t getByteValue(u16_t idx, u8_t off) const
    {
        ASSERT(idx < _valueCount && off < 4);
        return *(reinterpret_cast<u8_t*>(&_dwordValue[idx])+off);
    }

    void setDwordValue(u16_t idx, u32_t v);
    void setQwordValue(u16_t idx, const u64_t& v);
    void setFloatValue(u16_t idx, f32_t v);
    void setWordValue(u16_t idx, u8_t off, u16_t v);
    void setByteValue(u16_t idx, u8_t off, u8_t v);

private:
    inline void initValues()
    {
        if (_dwordValue)
            return;
        _dwordValue = new u32_t[_valueCount];
        MEMZRO(_dwordValue, _valueCount*sizeof(u32_t));
    }

protected:
    Pos _pos;
    PosI _grid;
    bool _inWorld;
    int _objectType;

    u16_t _valueCount; // XXX: start from 0
    union
    {
        u32_t* _dwordValue; // XXX: base value space

        u64_t* _qwordValue;
        f32_t* _floatValue;
        u16_t* _wordValue;
        u8_t* _byteValue;
    };
    BitVec _updateMask;
    char _name[NAME_MAX+1];
};

} // namespace buf

#endif // GAMEOBJECT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

