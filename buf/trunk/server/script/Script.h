
#ifndef SCRIPT_H_
#define SCRIPT_H_

extern "C" 
{
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

#include "lua_tinker.h"
#include <string>

using namespace lua_tinker;

namespace buf
{

typedef lua_tinker::table Table;
typedef lua_State State;

class Script
{
public:
    Script(const char* name = "Script");
    virtual ~Script();

    virtual bool init() { return true; }
    const char* getName() const { return _name; }

    inline bool doFile(const char* filename)
    {
        if (_L)
            return dofile(_L, filename);
        return false;
    }
    inline bool doString(const char* str)
    {
        if (_L)
            return dostring(_L, str);
        return false;
    }
    inline bool doBuffer(const char* buf, size_t size)
    {
        if (_L)
            return dobuffer(_L, buf, size);
        return false;
    }

    inline bool doFile(const std::string& filename) { return doFile(filename.c_str()); }
    inline bool doString(const std::string& str) { return doString(str.c_str()); }
    inline bool doBuffer(const std::string& buf) { return doBuffer(buf.c_str()); }

    template<typename T>
    inline T call(const char* name)
    {
        return lua_tinker::call<T>(_L, name);
    }
    template<typename T, typename T1>
    inline T call(const char* name, T1 arg1)
    {
        return lua_tinker::call<T>(_L, name, arg1);
    }
    template<typename T, typename T1, typename T2>
    inline T call(const char* name, T1 arg1, T2 arg2)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2);
    }
    template<typename T, typename T1, typename T2, typename T3>
    inline T call(const char* name, T1 arg1, T2 arg2, T3 arg3)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2, arg3);
    }
    template<typename T, typename T1, typename T2, typename T3, typename T4>
    inline T call(const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2, arg3, arg4);
    }
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline T call(const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2, arg3, arg4, arg5);
    }
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    inline T call(const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    inline T call(const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    inline T call(const char* name, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
    {
        return lua_tinker::call<T>(_L, name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }

    template<typename T>
    inline void set(const char* name, T object)
    {
        lua_tinker::set(_L, name, object);
    }

    template<typename T>
    inline T get(const char* name)
    {
        return lua_tinker::get<T>(_L, name);
    }

    template<typename T>
    inline Script& decl(const char* name, T object)
    {
        lua_tinker::decl(_L, name, object);
        return *this;
    }

    template<typename F>
    inline Script& def(const char * name, F func)
    {
        lua_tinker::def(_L, name, func);
        return *this;
    }

    template <typename T>
    inline Script& class_add(const char* name)
    {
        lua_tinker::class_add<T>(_L, name);
        return *this;
    }

    template <typename T, typename B>
    inline Script& class_inh(const char* name)
    {
        lua_tinker::class_inh<T, B>(_L);
        return *this;
    }

    template <typename T, typename F>
    inline Script& class_def(const char* name, F f)
    {
        lua_tinker::class_def<T>(_L, name, f);
        return *this;
    }

    template <typename T, typename B, typename V>
    inline Script& class_mem(const char* name, V B::*v)
    {
        lua_tinker::class_mem<T>(_L, name, v);
        return *this;
    }

    inline State* getState() { return _L; }

protected:
    State* _L;
    char _name[NAME_MAX+1];
};

} // namespace buf

#endif // SCRIPT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

