
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include "core/XML.h"
#include <vector>
using namespace buf;

int main(int argc, char* argv[])
{
    (void)argc,(void)argv;
    XML p;
    //if (p.open("/tmp/test.xml", "utf-8"))
    if (p.open("test.xml", "GBK"))
        printf("open ok\n");
    else 
        printf("open not ok\n");

    XML::node_pointer root = p.getRoot();
    if (root) {
        printf("root: %s\n", root->name);
        XML::node_pointer child = p.getChild(root);
        std::vector<char> buffer(1024);
        while (child) {
            printf("child: %s\n", child->name);
            p.getPropertyString(child, "name", &buffer[0], buffer.capacity());
            printf("\t name: %s\n", &buffer[0]);
            buffer[0] = '\0';
            //p.getContentString(child, &buffer[0], buffer.capacity());
            int f = 0;
            long lf = 0;
            p.getContentNum(child, &f);
            p.getContentNum(child, &lf);

            int pn = 0;
            p.getPropertyNum(child, "value", &pn);
            printf("\t CTX: %d %ld, property: %d\n", f, lf, pn);
            child = p.nextNode(child);
            buffer[0] = '\0';
        }
    }

    p.newChild(p.getRoot(), "newnode", "OKOKOKOKOKOK");
    p.saveas("test1.xml");

	return 0;
}

