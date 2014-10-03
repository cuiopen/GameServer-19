
#ifndef ENUMERATOR_H_
#define ENUMERATOR_H_

/**
 * TODO: When I is map<K, V>::iterator, you should define
 * a subclass of each with T = map<K, V>::value_type
 **/

template <typename T>
class Enumerator
{
public:
    virtual ~Enumerator() {}
    virtual bool operator()(T*) = 0;
    virtual bool operator()(T&) { return false; }
};

#endif // ENUMERATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

