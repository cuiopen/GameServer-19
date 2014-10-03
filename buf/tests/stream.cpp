
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Config.h"
#include "core/ByteBuffer.h"
#include "core/StringBuffer.h"
#include "core/Stream.h"

using namespace buf;

void test(void)
{
    ByteBuffer st;

    st << 'c';
    st << 19;

    printf("%c, %d\n", st[0], st.at<int>(1));
    st.at<char>(0) = 'a';

    u8_t c;
    st >> c;
    int v;
    st >> v;
    printf("%c, %d\n", c, v);
}

void test1(void)
{
    const char* str = "hello";
    ByteBuffer st((u8_t*)str, strlen(str));
    st << " world";
    st << 1;
    st << (short)2;
    st << (unsigned long)-1;
    st.rflip();
    printf("%u: %s\n", st.rsize(), &st);
}

void test2(void)
{
    StringBuffer st;
    st << 100;
    st << " fuck 妈的";
    printf("%s\n", &st);
}

void test3(void)
{
    ByteBuffer st;
    int v;
    st << 100;
    printf("st size: %u\n", st.rsize());
    st << 1000;
    printf("st size: %u\n", st.rsize());
    st >> v;
    printf("%d\n", v);
    st >> v;
    
    std::string name;
    st << "yangyoufa";
    st >> name;
    printf("name: %s\n", name.c_str());

}

void test4()
{
    Buffer st;
    std::string str = "yangyoufa";
    std::string str1;
    st << str;
    st >> str1;

    printf("%s\n", str1.c_str());
}

void test5()
{
    Stream<int> st;
    int a = 999, a1 = 123, a2 = 444;
    st.put(&a, 1);
    st << a1;
    st << a2;

    int a3;
    st >> a3;
    printf("%d\n", a3);
    st.remove(1, 4);

    st.rflip();

    st << 1 << 2 << 3;

    int b;
    st >> b;
    int c;
    st.read(c);

    printf("c:%d, b:%d\n", b, c);
}

int main(int argc, char* argv[])
{
    (void)argc;(void)argv;
#if 0
    test();
    test1();
    test2();
    test3();
    test4();
#endif
    test5();
	return 0;
}
