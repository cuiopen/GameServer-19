
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/Encoding.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Encoding conv;
    if (conv.open("utf-8", "GBK")) {
        printf("utf-8 to GBK open OK\n");
        const char* str = "这是不可能的";
        size_t len = strlen(str);
        printf("BEFORE CONVERTED: %s LEN: %lu\n", str, len);
        conv.encoding(str, len);
        printf("CONVERTED: %s LEN: %lu\n", &conv.data()[0], conv.size());
    }

    Encoding conv1;
    if (conv1.open("GBK", "utf-8")) {
        printf("GBK to utf-8 open OK\n");
        size_t len = conv.size();
        const char* str = &conv.data()[0];
        conv1.encoding(str, len);
        printf("CONVERTED: %s LEN: %lu\n", &conv1.data()[0], conv1.size());
    }

	return 0;
}
