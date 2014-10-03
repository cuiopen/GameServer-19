
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include <vector>

int main(int argc, char* argv[])
{
    printf("sizeof(char): %lu\n", sizeof(char));
    printf("sizeof(unsigned char): %lu\n", sizeof(unsigned char));
    printf("sizeof(short): %lu\n", sizeof(char));
    printf("sizeof(unsigned short): %lu\n", sizeof(unsigned short));
    printf("sizeof(int): %lu\n", sizeof(int));
    printf("sizeof(unsigned int): %lu\n", sizeof(unsigned int));
    printf("sizeof(long): %lu\n", sizeof(long));
    printf("sizeof(unsigned long): %lu\n", sizeof(long));
    printf("sizeof(long long): %lu\n", sizeof(long long));
    printf("sizeof(unsigned long long): %lu\n", sizeof(unsigned long long));
    printf("sizeof(float): %lu\n", sizeof(float));
    printf("sizeof(double): %lu\n", sizeof(double));
    printf("sizeof(long double): %lu\n", sizeof(long double));
    printf("sizeof(size_t): %lu\n", sizeof(size_t));
    printf("sizeof(void*): %lu\n", sizeof(void*));

    printf("sizeof(std::vector<int>): %lu\n", sizeof(std::vector<int>));
    printf("sizeof(std::vector<long long>): %lu\n", sizeof(std::vector<long long>));
	return 0;
}
