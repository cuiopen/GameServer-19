
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "script/Script.h"
#include "core/RTime.h"

using namespace buf;

void test1()
{
    Script s;
    s.doString("print('hello world!')");

    {
        TimeFilm tf;
        for (int i = 0; i < 1000000; ++i)
        {
            s.set("_fuck", 100);
            s.get<bool>("_fuck");
        }
    }

    {
        TimeFilm tf;
        s.doString("function test() local a = 100 * 100; b = a + 100 end");
        s.call<void>("test");
        for (int i = 0; i < 1000000; ++i)
        {
            s.call<void>("test");
        }
    }
}

class Base;

class Base
{
public:
    Base() : a(999)
    {
        fprintf(stderr, "%p: %s\n", (void*)this, __PRETTY_FUNCTION__);
    }
    ~Base()
    {
        fprintf(stderr, "%p: %s\n", (void*)this, __PRETTY_FUNCTION__);
    }

    void print()
    {
        fprintf(stderr, "%p: %s\n", (void*)this, __PRETTY_FUNCTION__);
    }

    void print_v(int v)
    {
        fprintf(stderr, "%p: %s: %d\n", (void*)this, __PRETTY_FUNCTION__, v);
    }

    static void print_s(Base* b, int a, int c = 100)
    {
        fprintf(stderr, "%p: %s: %d, %d\n", (void*)b, __PRETTY_FUNCTION__, a, c);
    }

    int a;
};

void test2()
{
    Script s;
    Base b;

    fprintf(stderr, "b: %p\n", (void*)&b);
    s.class_add<Base>("Base");
    s.class_def<Base>("print", &Base::print);
    s.class_def<Base>("print_v", &Base::print_v);
    s.class_mem<Base>("mem_a", &Base::a);
    //s.decl("base", &b);
    s.set("base", &b);

    s.def("print_s", &Base::print_s);

    s.doString("base:print()");
    s.doString("base:print_v(100)");
    s.doString("print(base.mem_a);");
    b.a = 123;
    s.doString("print(base.mem_a);");

    s.call<void>("print_s", &b, 99, 12);
    s.call<void>("print_s", &b, 929,1);
}

void test3()
{
    Script s;
    s.class_add<Base>("Base");
}

void test4()
{
    Script s;
    s.doFile("test.lua");
    s.doString("print(__s64);");
    s.set("longval", 0xFFFFFFFFFLLU);
    s.set("longval_1", 0xFFFFFFFFFFLLU);
    s.set("longval_2", 0xFFFFFFFFLLU);
    s.set("longval_3", 0xFFFFFFFFFLLU);
    s.set("longval_4", 0xFFFFFFFFFLLU);
    s.doString("print('longval:' .. tostring(longval))");
    s.doString("print('longval_1:' .. tostring(longval_1))");
    s.doString("print('longval_2:' .. tostring(longval_2))");
    s.doString("print('longval_3:' .. tostring(longval_3))");
    s.doString("print('longval_4:' .. tostring(longval_4))");
    s.doString("print('longval < longval_1:' .. tostring(longval < longval_1))");
    s.doString("print('longval_2 < longval:' .. tostring(longval_2 < longval))");
    s.doString("print('longval <= longval_3:' .. tostring(longval <= longval_3))");
    s.doString("print('longval == longval_4:' .. tostring(longval == longval_4))");
    s.doString("print('longval == longval_1:' .. tostring(longval == longval_1))");
    s.doString("print('longval_1 <= longval:' .. tostring(longval_1 <= longval))");
}

extern "C" int puts(const char* fmt);

extern "C"
{
inline int putss(const char* fmt)
{
    return puts(fmt);
}
}

void test5()
{
    Script s;
#if 1
    s.def("puts", &putss);
    {
        TimeFilm tm("");
        s.doString("for n=1,10000000 do puts(\"\") end");
    }
#endif

    s.doString("ffi=require(\"ffi\");ffi.cdef[[int puts(const char* msg);]]");
#if 1
    {
        TimeFilm tm("");
        s.doString("for n=1,10000000 do ffi.C.puts(\"\") end");
        //s.doString("for n=1,1 do ffi.C.puts(\"\") end");
    }
#endif
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test1();
    test2();
    test3();
    test4();
    //test5();
	return 0;
}

