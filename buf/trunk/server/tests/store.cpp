
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "common/Store.h"
using namespace buf;

int main(int argc, char* argv[])
{
    Store<int> store;
    StoreItem<int> i;
    store.add(0, i);

	return 0;
}
