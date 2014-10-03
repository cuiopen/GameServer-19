
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Url.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc;(void)argv;
    Url url("mysql://user:password@host:8888w/db?param1&param2");
    printf("%s, %s, %s, %u, %s\n",
            url.user(), url.passwd(), url.host(), url.port(), url.db());
    const std::vector<std::string>& params = url.parameters();
    for (size_t i = 0; i < params.size(); ++i)
        printf("%s\n", params[i].c_str());
	return 0;
}
