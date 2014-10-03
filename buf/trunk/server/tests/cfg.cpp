
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "common/Cfg.h"
using namespace buf;

int main(int argc, char* argv[])
{
    Cfg cfg;
    //cfg.load("test.lua");
    //cfg.reload();
    //cfg.set("yangyoufa", 100);
    //int a = cfg.get<int>("yangyoufa");
    //printf("%d\n", a);
    //int b = cfg.get<int>("bbb");
    //printf("%d\n", b);
    //int c = cfg.get<int>("bbc");
    //printf("%d\n", c);

    if (cfg.load("config.lua"))
    {
        printf("OK\n");
    }
    printf("%d\n", cfg.get<int>("a"));
    Table master = cfg.get<Table>("master");
    const char* ip = master.get<const char*>("ip");
    printf("ip: %s\n", ip);
    //printf("%s\n", master.get<const char*>("ip"));
    const char* nnn = master.get<const char*>("nnn");
    printf("core: %s\n", nnn);

    gCfg.setAllowedIP("192.168.1.1/24");
    if (gCfg.isIPAllowed("192.168.1.132"))
        fprintf(stderr, "OK\n");
    if (!gCfg.isIPAllowed("192.168.2.132"))
        fprintf(stderr, "NO\n");

    Table xx = cfg.get<Table>("xx");
    if (!xx.validate())
        fprintf(stderr, "xx is not validate\n");

	return 0;
}
