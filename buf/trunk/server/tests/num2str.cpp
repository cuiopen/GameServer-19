
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Utils.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    char buf[1024] = {0};
    num2str('a', buf, sizeof(buf));
    printf("%s\n", buf);
    num2str(100, buf, sizeof(buf));
    printf("%s\n", buf);
    num2str((unsigned)19999, buf, sizeof(buf));
    printf("%s\n", buf);
	return 0;
}
