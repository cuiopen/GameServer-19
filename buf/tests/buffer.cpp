
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Buffer.h"
//#include "core/StackBuffer.h"
//#include "core/MemReader.h"
using namespace buf;

void test()
{
    Buffer buf;
    //buf << "hello";
    std::string tmp = "xxxxxx-";
    buf << tmp;
    std::string tmp1;
    buf >> tmp;
    buf.put("hello-", 6);
    buf.put("hello-", 6);
    buf.put("hello-", 6);
    buf.put("hello-", 6);
    buf.put("hello-", 6);
    //buf << 111111;
    printf("%s\n", buf.rbuf());
    printf("%s\n", tmp.c_str());
    buf.clear();
    printf("%s\n", buf.rbuf());
}

void test1()
{
    Buffer b;
    b << "fuck";
    b << 100;
    b << 999;
    b << static_cast<short>(7);
    b << "you";

    std::string s1;
    int i1;
    int i2;
    short u1;
    std::string s2;

    b >> s1 >> i1 >> i2 >> u1 >> s2;

    printf("%s, %d, %d, %d, %s\n", s1.c_str(), i1, i2, u1, s2.c_str());
}

void test2()
{
    Buffer b;
    b << 100;
    int i1;
    b >> i1;

    b << "yangyoufa";
    std::string s1;
    b >> s1;

    b << 100;
    b.rflip(4);
    size_t s = b.curr();
    b << 90;
    b.remove(s, 4);

    size_t off = b.curr();
    b << 0;
    b.at<int>(off) = 999;

    int i2, i3, i4;
    b >> i2;
    b >> i3 >> i4;
    printf("%s, %d, %d\n", s1.c_str(), i1, i2);
}

void test3()
{
    StackBuffer<> sb("hello", 5);
    StackBuffer<> sb1(sb);

    printf("%s(%u)\n", sb.rbuf(), sb.rsize());
    sb.remove(0, 7);
    printf("%s(%u)\n", sb.rbuf(), sb.rsize());

    printf("%s(%u)\n", sb1.rbuf(), sb1.rsize());
    sb1.remove(1,2);
    printf("%s(%u)\n", sb1.rbuf(), sb1.rsize());
}

void test4()
{
    Buffer sb("hello", 5);
    Buffer sb1(sb);

    printf("sizeof(Buffer): %lu\n", sizeof(Buffer));

    printf("sb: %s\n", sb.rbuf());
    sb.remove(0, 7);
    printf("sb: %s(%d)\n", sb.rbuf(), sb.rsize());

    printf("sb1: %s\n", sb1.rbuf());
    sb1.remove(1,2);
    printf("sb1: %s(%d)\n", sb1.rbuf(), sb1.rsize());
}

void test5()
{
    char buf[128];
    strlen_t* pbuf = (strlen_t*)&buf[0];
    *pbuf = 3;
    memcpy(buf+sizeof(*pbuf), "YYF", 3);
    MemReader mr(&buf[0], sizeof(buf));
    std::string name;
    mr >> name;

    printf("name: %s\n", name.c_str());
}

void ntest()
{
    BufferWriter<BufferContainer> b(100);
    int a;
    b >> a;
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    test1();
    test2();
    test3();
    test4();
    test5();

	return 0;
}
