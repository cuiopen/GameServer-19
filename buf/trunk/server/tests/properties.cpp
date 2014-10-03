
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <iostream>

#include "core/Properties.h"
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    Properties p;
    p["hello"] = "yangyoufa";
    std::cout << "hello: " << p["hello"] << std::endl;

    if (p["hello"] != "yangyoufa")
        std::cout << "error" << std::endl;
    if (p["yangyoufa"] == "") {
        std::cout << "no key is yangyoufa" << std::endl;
        p["yangyoufa"] = "oxoxox";
    }   
    std::cout << "yangyoufa: " << p["yangyoufa"] << std::endl;

    return 0;
}
