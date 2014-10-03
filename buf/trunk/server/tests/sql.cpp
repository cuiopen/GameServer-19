
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/DBMysqlConnection.h"
#include <mysql.h>
using namespace buf;

#if 0
void test1()
{
    Url url = "mysql://bsc:123456@localhost:3306/asss_8888";
    DBMysqlConnection c(DBMysqlConnection::createDescription(url));
    //if (!c.connect("localhost", "bsc", "123456", "data", 3306)) {
    if (!c.connect(url)) {
        printf("ERROR\n");
        return;
    }
    c.setState(DBCS_QUERYING);

    //c.vquery("update acupra set pramax = pramax + %u", 10000);
    //printf("%d\n", c.count("special_fighter_template"));
    //printf("%d\n", c.sqlDelete("special_fighter_template", NULL));

    //c.sqlSelect("*", "special_fighter_template", buffer, 100, "id=10000", "id", "lvl");
    //c.list();

    //char buffer[64*1024] = {0};
    //c.sqlSelect("*", "skills", buffer, 1);

    Buffer buf;
    c.vquery(buf, "select a.name,b.id from player as a, fighter as b where a.id = b.playerId");

    c.close();
}
#endif

void test2()
{
    //Url url = "mysql://bsc:123456@localhost:3306/a";
    Url url = "mysql://kingxin:kingxin@192.168.1.102:3306/a";
    DBMysqlConnection c(DBMysqlConnection::createDescription(url));
    //if (!c.connect("localhost", "bsc", "123456", "data", 3306)) {
    if (!c.connect(url)) {
        printf("ERROR\n");
        return;
    }
    c.setState(DBCS_QUERYING);

    Buffer buf;
    c.sqlSelect("test", "*", buf);

    c.setState(DBCS_QUERYING);
    c.close();
}

void test3()
{
    Url url = "mysql://root:root@localhost:3306/test";
    DBMysqlConnection c(DBMysqlConnection::createDescription(url));
    //if (!c.connect("localhost", "bsc", "123456", "data", 3306)) {
    if (!c.connect(url)) {
        printf("ERROR\n");
        return;
    }
    c.setState(DBCS_QUERYING);

    c.setState(DBCS_QUERYING);
    c.close();
}

int main(int argc, char* argv[])
{
    (void)argc;(void)argv;
    //test1();
    //test2();
    test3();
	return 0;
}

