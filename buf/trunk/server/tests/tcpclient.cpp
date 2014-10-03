
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/TcpClient.h"
using namespace buf;

int test1()
{
    TcpClient cl("Funk", "127.0.0.1", 7777);

    static unsigned long long i = 0;
    if (cl.connect()) {
        while (true)
        {
#if 1
            char buf[128] = {0};
            int n = SNPRINTF(buf, sizeof(buf), "hello 111111111111111111111111111111111111111111111111111- %llu", i++);
#if 0
            char buffer[128] = {0};
            unsigned short* pbuf = (unsigned short*)buffer;
            *pbuf = 1;
            memcpy(pbuf+1, buf, n);
            if (cl.sendPack(buffer, strlen(buffer)) < 0)
                break;
#else
            if (cl.sendPack(buf, strlen(buf)) < 0)
                break;
#endif
            sleep(1);

            //cl.recvPack();
        }   
#endif
    }
    else
    {
        printf("connecting ERROR\n");
    }

    cl.join();
	return 0;
}

int test2()
{
    TcpClient cl("Funk", "127.0.0.1", 7777);

    if (cl.connect()) {
        while (true)
        {
            char buf[1024] = {0};
            char buffer[1024] = {0};
            int n = SNPRINTF(buf, sizeof(buf), "<pol");
            if (cl.send(buf, strlen(buf)) < 0)
                break;
            //sleep(1);
            int ret = 0;
            if ((ret = cl.recv(buffer, 1024)) < 0)
                break;
            if (ret > 0)
                printf("%s\n", buffer);
        }
    }


    cl.join();
    return 0;
}

void test3()
{
    TcpClient cl("Funk", "127.0.0.1", 7777);
    if (cl.connect())
    {
        cl.start();
        Buffer b;
        b << "hello";
        cl.sendPack(b.rbuf(), b.rsize());
        cl.join();
    }
}

int main(int argc, char* argv[])
{
    (void)argc;(void)argv;
    //test1();
    //test2();
    test3();
	return 0;
}

