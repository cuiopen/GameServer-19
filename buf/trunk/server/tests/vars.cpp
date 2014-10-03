
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
}

#include "common/Vars.h"

using namespace buf;

int main(int argc, char* argv[])
{
    Vars<> vars;

    vars.setVar(1, 100);
    printf("%d\n", vars.getVar(1));
    vars.setVar(1, 1000);
    printf("%d\n", vars.getVar(1));
    vars.setVar(1, 1000, VARRECYCLE_MIN); 
    sleep(65);
    printf("%d\n", vars.getVar(1));
    //vars.delVar(1);

    vars.setVar(2, 888);
    printf("%d\n", vars.getVar(2));

    Buffer buf;
    vars.serialize(buf);
    vars.clear();
    vars.unserialize(buf);
    printf("%d\n", vars.getVar(1));
    printf("%d\n", vars.getVar(2));
	return 0;
}

