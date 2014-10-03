
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/DBMysqlConnectionPool.h"
#include "core/Buffer.h"
using namespace buf;

int test_main(int argc, char* argv[])
{
    (void)argc;(void)argv;
    DBMysqlConnectionPool pool("mysql://bsc:123456@localhost:3306/data");
    if (pool.init()) {
#if 0
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
        pool.push("DELETE FROM acupra");
#endif
        DBConnection* conn = pool.get();
        if (conn) {
            struct Record
            {
                short v1;
                char v2[255];
                char v3[255];
                char tblob[1];
                char blob[1];
            };

            char buffer[64*1024] = {0};
#if 0
            conn->vquery("CREATE TABLE `test` (\
                `id` smallint(5) NOT NULL,\
                `name` varchar(255) NOT NULL DEFAULT '',\
                `data` blob,\
                PRIMARY KEY (`id`)\
                ) ENGINE=MyISAM DEFAULT CHARSET=utf8;\
            ");
#endif
            conn->vquery("CREATE TABLE `a` (\
                `id` smallint(5) NOT NULL,\
                `name` varchar(255) NOT NULL DEFAULT '',\
                `name1` varchar(255) NOT NULL DEFAULT '',\
                `tblob` tinyblob,\
                `blob` blob,\
                PRIMARY KEY (`id`)\
                ) ENGINE=MyISAM DEFAULT CHARSET=utf8;\
            ");

            int a[10] = {0121,299,3,4,5,8736,732,328,739,23310};

            Buffer b;
            b << (short)101;
            b.put("yangyoufa2", 255);
            b.put("杨友发2", 255);
            b << (unsigned int)10 << (int)999 << (int)888 << (short)2;
            b << DB_ZIPSET((unsigned int)40);
            for (int i=0; i<10; ++i)
                b << a[i];

            conn->sqlInsert("a", "id,name,name1,tblob,blob", (const char*)b.rbuf(), 1);
#if 0
            //conn->sqlUpdate("a", "id,name,name1,tblob,blob", b.rbuf());
            //conn->sqlSelect("skills", "*", buffer, 0, 1, "id=101");

            //Buffer c;
            //c.put("yangyoufa4", 255);
            //c.put("杨友发4", 255);
            //conn->sqlUpdate("a", "name,name1", c.rbuf(), "id = 101");

            Buffer d;
            d << DB_ZIPSET((unsigned int)40);
            for (int i=0; i<10; ++i)
                d << a[i];
            conn->sqlUpdate("a", "blob", d.rbuf(), "id=101");

            Buffer bu;
            conn->sqlSelect("a", "*", bu, 0, 1, "id = 101");
            Record* ret = (Record*)bu.rbuf();

            fprintf(stderr, "v1:%d, v2:%s, v3:%s\n", ret->v1, ret->v2, ret->v3);
            fprintf(stderr, "tblob size: %u, %d, %d, %d\n", *(unsigned int*)ret->tblob,
                    *(int*)(ret->tblob+4), *(int*)(ret->tblob+8), *(short*)(ret->tblob+12));
            fprintf(stderr, "blob size: %u ", *(unsigned int*)(ret->tblob+14));
            int* pval = (int*)(ret->tblob+18);
            for (int i=0; i<10; ++i)
                fprintf(stderr, "%d,", *pval++);
            fprintf(stderr, "\n");
#endif

            pool.put(conn);
        }

    }

    pool.join();
}

void test1()
{
    //DBMysqlConnectionPool pool("mysql://kingxin:kingxin@192.168.100.25:3306/a");
    DBMysqlConnectionPool pool("mysql://root:111111@localhost:3306/test");
    if (!pool.init())
        return;

    DBConnection* conn = pool.get();
    if (!conn)
        return;

    while (true)
    {
        conn->vquery("select * from `player` limit 5;");
        fprintf(stderr, "sleeping...\n");
        Thread::sleep(1);
    }
    pool.put(conn);
}

void test2()
{
    DBMysqlConnectionPool pool("mysql://root:111111@localhost:3306/test");
    if (!pool.init())
        return;

    DBConnection* conn = pool.get();
    if (!conn)
        return;

    std::string str = "select * from `player` where id = 100 or playerid=111 %s or name like '%s' or name lime \"xx\";";
    printf("before: %s\n", str.c_str());
    conn->escape(str);
    printf("after: %s\n", str.c_str());

    pool.put(conn);
}

void test3()
{
    DBMysqlConnectionPool pool("mysql://root:111111@localhost:3306/test");
    if (!pool.init())
        return;

    const char* sql = "select * from one1;select * from test1;select * from test;";
    const char* sql1 = "select * from test;";
    while (true)
    {
        int i = 0;
        while (i++ < 100)
        {
            if (i%2)
                pool.push(sql, strlen(sql));
            else
                pool.push(sql1, strlen(sql1));
        }
        Thread::sleep(1);
    }
}

int main(int argc, char* argv[])
{
    //test_main(argc, argv);
    //test1();
    //test2();
    test3();
	return 0;
}
