
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <vector>

int main(int argc, char* argv[])
{
    std::vector<int> l(10);

    printf("%lu\n", l.size());
    l.erase(l.begin()+1, l.begin()+2);
    printf("%lu\n", l.size());
	return 0;
}
