
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Utils.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    char buf[32] = {"      "};
    trim(buf);
    printf("%s\n", buf);

    std::string str = "   hello    ";
    trim(str);
    printf("%s KKK\n", str.c_str());


	return 0;
}
