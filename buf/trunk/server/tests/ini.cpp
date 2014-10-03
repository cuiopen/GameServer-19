
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/INI.h"
#include "core/Utils.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    INI ini;
    ini.open("test.ini");

    INI::iterator it = ini.begin();
    INI::iterator end = ini.end();
    for (; it != end; ++it) {
        INI::giterator git = it->second.begin();
        INI::giterator gend = it->second.end();
        for (; git != gend; ++git) {
            printf("%s, %s\n", git->first.c_str(), git->second.c_str());
        }
    }

	return 0;
}
