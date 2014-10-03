
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <iostream>

#include "core/Tokenizer.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Tokenizer tk("a,b,c", ',');
    std::cout << "size: " << tk.size() << std::endl;

    for (size_t i = 0; i < tk.size(); ++i)
    {
        std::cout << "tk[" << i << "]: " << tk[i] << std::endl;
    }

    Tokenizer tk1("a,,,b,,,c,,,", ",,");
    for (size_t i = 0; i < tk1.size(); ++i)
    {
        std::cout << "tk1[" << i << "]: " << tk1[i] << std::endl;
    }
	return 0;
}
