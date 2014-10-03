
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/WordFilter.h"
#include "core/RTime.h"
using namespace buf;

int main(int argc, char* argv[])
{
    WordFilter::getInstance().init("./words.txt");

    char buf[128] = {0};
    snprintf(buf, sizeof(buf), "我日本我啊");
    printf("buf: %s\n", buf);
    WordFilter::getInstance().filter(buf, strlen(buf));
    printf("buf: %s\n", buf);
    snprintf(buf, sizeof(buf), "我日你江泽民");
    printf("buf: %s\n", buf);
    WordFilter::getInstance().filter(buf, strlen(buf));
    printf("buf: %s\n", buf);

    {
        {
            TimeFilm tm("");
            for (int i = 0; i < 1000; ++i)
            {
                char buf[128] = {0};
                snprintf(buf, sizeof(buf), "我日本我啊");
                WordFilter::getInstance().filter(buf, strlen(buf));
            }
        }
    }

	return 0;
}
