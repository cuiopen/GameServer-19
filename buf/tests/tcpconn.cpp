
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/TcpConnection.h"
#include "core/Buffer.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    //TcpConnection conn("192.168.100.25", 8888);
    //TcpConnection conn("s66.app27036.qqopenapp.com", 443);
    //TcpConnection conn("121.14.186.160", 443);
    //TcpConnection conn("192.168.0.147", 7777);
    TcpConnection conn("127.0.0.1", 7777);

#if 0
    if (conn.connect()) {
        Buffer buf;
        buf << static_cast<s16_t>(9);
        buf << static_cast<u8_t>(0);
        buf << static_cast<u8_t>(0xfd);
        buf << static_cast<u8_t>(1);
        buf << static_cast<u64_t>(34535643);
        conn.send(buf.rbuf(), buf.rsize());
        buf.clear();
        int ret = 0;
        do {
            ret = conn.recv((char*)buf.wbuf(), buf.wsize());
            if (ret > 0)
                buf.wflip(ret);
        } while (true);
        printf("%d,%lu: %s\n", ret, buf.rsize(), buf.rbuf());
    }
#endif

#if 0
    if (conn.connect()) {
        Buffer buf;
        Buffer wbuf;
        //buf << "GET / HTTP/1.1\r\nHost: s278.app27036.qqopenapp.com:443\r\n\r\n";
#if 0
        buf << "GET / HTTP/1.1\r\nHost: s278.app27036.qqopenapp.com:443\r\n\r\n";
        buf << static_cast<s16_t>(9);
        buf << static_cast<u8_t>(0);
        buf << static_cast<u8_t>(0xe0);
        buf << static_cast<u8_t>(1);
#endif
        buf << "<pol";
        //buf << static_cast<u64_t>(34535643);
        int ret = 0;
        do {
            conn.send(buf.rbuf(), buf.rsize());
            ret = conn.recv((char*)wbuf.wbuf(), wbuf.wsize());
            if (ret > 0)
                wbuf.wflip(ret);
            if (!conn.connected())
                break;
        } while (true);
        printf("%d,%lu: %s\n", ret, wbuf.rsize(), wbuf.rbuf());
    }
#endif

#if 0
    if (conn.connect()) {
        Buffer buf;
        conn.send("\0\0\0\010GETffffffffff1", 18);
        //sleep(1);
        int ret = 0;
        do {
            ret = conn.recv((char*)buf.wbuf(), buf.wsize());
            if (ret > 0)
                buf.wflip(ret);
            else
                break;
        } while (true);
        printf("%d,%lu: %s\n", ret, buf.rsize(), buf.rbuf());
    }
#endif

#if 0
    if (conn.connect()) {
        conn.start();
        conn.join();
    }
#endif

#if 1
    static unsigned long long i = 0;
    if (conn.connect()) {
        char buf[128] = {0};
        while (true) {
            SNPRINTF(buf, sizeof(buf), "hello 111111111111111111111111111111111111111111111111111- %llu", i++);
#if 1
            if (conn.sendPack(buf, strlen(buf)) < 0)
                break;
#else
            conn.sendPack(buf, strlen(buf));
#endif
            sleep(1);

            //conn.recvPack();
        }
    } else {
        printf("connecting ERROR\n");
    }
#endif

	return 0;
}

