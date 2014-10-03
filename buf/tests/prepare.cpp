
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/DBMysqlConnectionPool.h"
#include "core/Buffer.h"
using namespace buf;


DBODEF(DBObject, 1, char, c);

DBODEF(DBObject1, 2, char, c, unsigned char, d); 
DBODEF(DBObject2, 3, char, c, unsigned char, d, std::string, s); 
DBODEF(DBObject3, 4, char, c, unsigned char, d, short, e, std::string, s); 

DBODEF(DBObject14, 15,
    char, c,
    unsigned char, d,
    short, e,
    unsigned short, f,
    int, g,
    unsigned int, h,
    long long, i,
    unsigned long long, j,
    float, k,
    float, kk,
    double, l,
    std::vector<unsigned char>, t, // use t.size() to test if have data
    std::string, m, // use m.size() to test if have data
    Buffer, n, // use n.rsize() to test if have data
    unsigned long, tt
    ); 

void test2()
{
    DBMysqlConnectionPool pool("mysql://root:111111@localhost:3306/test");
    //DBMysqlConnectionPool pool("mysql://root:111111@172.16.5.1:3306/test");
    if (!pool.init())
        return;

    DBConnection* conn = pool.get(DBCS_PREPARING);
    if (!conn)
        return;

    DBObject a;
    //conn->prepare("select * from one", a);
    conn->prepare("select * from one", a);
    while (!conn->next())
    {
        printf("%d\n", a.c);
    }

    printf("---------------------------------\n");
    pool.put(conn);
}

void test3()
{
    DBMysqlConnectionPool pool("mysql://root:111111@localhost:3306/test");
    if (!pool.init())
        return;

    DBConnection* conn = pool.get(DBCS_PREPARING);
    if (!conn)
        return;

    DBObject14 a;
    if (!conn->prepare("select *,unix_timestamp(t) from test", a))
    {
        while (!conn->next())
        {
            //printf("a.m.size(): %lu\n", a.m.size());
            //printf("a.n.rsize(): %d\n", a.n.rsize());
            printf("%lu,%d,%d,%d,%d,%d,%d,%lld,%lld,%f,%f,%f,%s,%s(%d)\n", a.tt,a.c,a.d,a.e,a.f,a.g,a.h,a.i,a.j,a.k,a.kk,a.l,&a.m[0],a.n.rbuf(),a.n.rsize());
        }
    }

    printf("---------------------------------\n");
#if 1
    conn->finish();
    DBObject14 b;
    if (!conn->prepare("select *,unix_timestamp(t) from test", b))
    {
        while (!conn->next())
        {
            printf("%lu,%d,%d,%d,%d,%d,%d,%lld,%lld,%f,%f,%f,%s,%s(%d)\n", b.tt,b.c,b.d,b.e,b.f,b.g,b.h,b.i,b.j,b.k,b.kk,b.l,&b.m[0],b.n.rbuf(),b.n.rsize());
        }
    }
#endif

    conn->finish();
    pool.put(conn);


}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    test2();
    test3();
	return 0;
}
