
#ifndef MINHEAP_H_
#define MINHEAP_H_

#include "Config.h"

namespace buf
{

template<typename T, typename Greater>
struct MinHeapNode
{
    MinHeapNode(const T& val = T())
        : val(val), mhidx(-1), next(0), prev(0) {}
    ~MinHeapNode() {}

    inline T& getVal() { return val; }
    inline const T& getVal() const { return val; }

    inline bool operator>(const MinHeapNode& n) const
    {
        Greater gt;
        return gt(val, n.val);
    }

    // XXX: data
    T val;
    int mhidx;
    // TODO: we need this???
    struct MinHeapNode* next;
    struct MinHeapNode** prev;
};

template <typename T, typename Greater>
class MinHeap
{
public:
    typedef size_t size_type;
    typedef MinHeapNode<T, Greater> value_type;
    typedef value_type* pointer;

public:
    MinHeap() : _data(0), _size(0), _capacity(0) {}
    ~MinHeap();

    inline size_type size() const { return _size; }
    inline bool empty() const { return 0 == _size; }
    inline bool isTop(const pointer v) const { return v?v->mhidx==0:false; }
    inline pointer top() const { return _size?*_data:0; }

    int reserve(size_type n);
    pointer push(const T& v);
    pointer push(pointer v);
    pointer pop();
    int erase(const pointer v);

protected:
    void shiftUp(unsigned hole_index, const pointer v);
    void shiftDown(unsigned hole_index, const pointer v);

private:
    void* realloc(void* ptr, size_type size)
    {
        // TODO:
        return ::realloc(ptr, size);
    }

    void* malloc(size_type size)
    {
        // TODO:
        return ::malloc(size);
    }

    void free(void* ptr)
    {
        // TODO:
        ::free(ptr);
    }

private:
    pointer* _data;
    mutable size_type _size;
    size_type _capacity;
};

template <typename T, typename Greater>
MinHeap<T, Greater>::~MinHeap()
{
    for (unsigned i = 0; i < _size; ++i)
    {
        if (_data[i])
            DELETE(_data[i]);
    }
    free(_data);
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::pointer MinHeap<T, Greater>::push(const T& val)
{
    if (reserve(_size+1))
        return 0;
    pointer v = BUFNEW value_type(val);
    if (!v)
        return 0;
    shiftUp(_size++, v);
    return v;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::pointer MinHeap<T, Greater>::push(pointer v)
{
    if (reserve(_size+1))
        return 0;
    shiftUp(_size++, v);
    return v;
}

template <typename T, typename Greater>
typename MinHeap<T, Greater>::pointer MinHeap<T, Greater>::pop()
{
    if (_size)
    {
        pointer v = *_data;
        shiftDown(0, _data[--_size]);
        v->mhidx = -1;
        return v;
    }
    return 0;
}

template <typename T, typename Greater>
int MinHeap<T, Greater>::erase(const pointer v)
{
    if (!v)
        return -1;
    if (v->mhidx != -1)
    {
        pointer last = _data[--_size];
        unsigned parent = (v->mhidx - 1) / 2;
        if (v->mhidx > 0 && *_data[parent] > *last)
            shiftUp(v->mhidx, last);
        else
            shiftDown(v->mhidx, last);
        v->mhidx = -1;

        DELETE(v);
        return 0;
    }
    return -1;
}

template <typename T, typename Greater>
int MinHeap<T, Greater>::reserve(size_type n)
{
    if (_capacity < n)
    {
        size_type capacity = _capacity ? _capacity * 2 : 8;
        if (capacity < n)
            capacity = n;

        pointer* ptr = 0;
        if (!(ptr = (pointer*)realloc(_data, capacity*sizeof(pointer))))
            return -1;

        _data = ptr;
        _capacity = capacity;
    }
    return 0;
}

template <typename T, typename Greater>
void MinHeap<T, Greater>::shiftUp(unsigned hole_index, const pointer v)
{
    unsigned parent = (hole_index - 1) / 2;
    while (hole_index && (*_data[parent] > *v)) // XXX: >
    {
        (_data[hole_index] = _data[parent])->mhidx = hole_index;
        hole_index = parent;
        parent = (hole_index - 1) / 2;
    }
    (_data[hole_index] = v)->mhidx = hole_index;
}

template <typename T, typename Greater>
void MinHeap<T, Greater>::shiftDown(unsigned hole_index, const pointer v)
{
    unsigned min_child = 2 * (hole_index + 1);
    while (min_child <= _size)
    {
        min_child -= min_child == _size || (*_data[min_child] > *_data[min_child-1]);
        if (!(*v > *_data[min_child]))
            break;
        (_data[hole_index] = _data[min_child])->mhidx = hole_index;
        hole_index = min_child;
        min_child = 2 * (hole_index + 1);
    }
    (_data[hole_index] = v)->mhidx = hole_index;
}

} // namespace buf

#endif // MINHEAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

