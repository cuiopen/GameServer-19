
extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#include "core/JsonParser.h"
using namespace buf;

void test()
{
    json_t* arr = json_new_array();
    json_t* array = json_new_object();
    json_insert_pair_into_object(array, "array", arr);

    json_t* obj = json_new_object();
    json_insert_pair_into_object(obj, "[1]", json_new_number("100"));
    json_insert_pair_into_object(obj, "[2]", json_new_number("1212"));
    json_insert_pair_into_object(obj, "[3]", json_new_string("hello"));
    json_insert_child(arr, obj);

    json_t* obj1 = json_new_object();
    json_insert_pair_into_object(obj1, "[1]", json_new_number("100"));
    json_insert_pair_into_object(obj1, "[2]", json_new_number("1212"));
    json_insert_pair_into_object(obj1, "[3]", json_new_string("hello"));
    json_insert_pair_into_object(obj1, "[4]", json_new_null());
    json_insert_child(arr, obj1);


    char* str = 0;
    json_tree_to_string(array, &str);
    printf("%s\n", str);
}

void test1()
{
    JsonParser jp;
    jp.newTrue("true");
    jp.newFalse("false");

    Json* arr = jp.newArray("arr");

    Json* obj = jp.newObject(arr);
    jp.newString("hello", "yangyoufa", obj);
    jp.newNumber("a", 100, obj);

    Json* obj1 = jp.newObject(arr);
    jp.newString("hello", "yy", obj1);
    jp.newNull("", obj1);

    printf("%s\n", jp.toString());

    u32_t v;
    jp.getNumber(jp.find("a", obj), v);
    printf("%d\n", v);

    printf("%s\n", jp.getString(jp.find("hello", obj)));
}

int main(int argc, char* argv[])
{
    test();
    test1();
	return 0;
}
