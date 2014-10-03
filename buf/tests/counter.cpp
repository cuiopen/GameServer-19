
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/Counter.h"
using namespace buf;

int main(int argc, char* argv[])
{
    Counter c("Loading itemtemplate ");
    ++c;
    c++;
    c.finish();
	return 0;
}
