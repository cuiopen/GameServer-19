
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

#include "core/Foreach.h"
#include <vector>
#include <string>
#include <list>
#include <map>
using namespace buf;

void test()
{
    const char* buffer = "hello world!\n";
    struct MyEach : public Enumerator<const char>
    {
        virtual bool operator()(const char* c)
        {
            fprintf(stderr, "%c", *c);
            return true;
        }
    } exec;

#if 1
    foreach(buffer, buffer+strlen(buffer), exec);

    std::vector<char> vec;
    size_t len = strlen(buffer);
    vec.resize(len+1);
    strcpy(&vec[0], buffer);
    vec[len] = '\0';
    foreach(&vec[0], &vec[0]+vec.size(), exec);
    foreach(vec.begin(), vec.end(), exec);
    std::list<char> ls;
    //size_t len = strlen(buffer);
    for (size_t i=0; i<len; ++i) {
        ls.push_back(buffer[i]);
    }

    foreach(ls.begin(), ls.end(), exec);
#endif

#if 1
    typedef std::map<int, char> Map;
    typedef Map::iterator Iter;
    typedef Map::const_iterator CIter;
    typedef Map::value_type VType;

    struct MyEach1 : public Enumerator<VType>
    {
        virtual bool operator()(VType* i)
        {
            fprintf(stderr, "first: %d, second: %c\n", (*i).first, (*i).second);
            return true;
        }
    } exec1;

    Map m;
    for (size_t i=0; i<len; ++i) {
        m.insert(std::make_pair(i, buffer[i]));
    }

    foreach(m.begin(), m.end(), exec1);
#endif

	return;
}

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    test();
    return 0;
}
