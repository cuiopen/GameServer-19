
#ifndef REMOVETYPES_H_
#define REMOVETYPES_H_

#include <string>

namespace buf
{

// for remove types
template <typename T>
struct remove_pointer
{
    typedef T type;
};

template <typename T>
struct remove_pointer<T*>
{
    typedef T type;
};

template <typename T>
struct remove_const
{
    typedef T type;
};

template <typename T>
struct remove_const<const T>
{
    typedef T type;
};

template <typename T>
struct remove_pointer_const
{
    typedef T type;
};

template <typename T>
struct remove_pointer_const<const T*>
{
    typedef T type;
};

template <typename T>
struct remove_volatile
{
    typedef T type;
};

template <typename T>
struct remove_volatile<volatile T>
{
    typedef T type;
};
// end for remove types

// for is types
typedef unsigned long t_yes;
typedef unsigned char t_no;

template <typename T> struct is_bool { static const bool value = false; };
template <> struct is_bool<bool> { static const bool value = true; };

template <typename T> struct is_integer { static const bool value = false; };
template <> struct is_integer<bool> { static const bool value = true; };
template <> struct is_integer<const bool> { static const bool value = true; };
template <> struct is_integer<char> { static const bool value = true; };
template <> struct is_integer<const char> { static const bool value = true; };
template <> struct is_integer<short> { static const bool value = true; };
template <> struct is_integer<unsigned short> { static const bool value = true; };
template <> struct is_integer<const unsigned short> { static const bool value = true; };
template <> struct is_integer<int> { static const bool value = true; };
template <> struct is_integer<unsigned int> { static const bool value = true; };
template <> struct is_integer<const unsigned int> { static const bool value = true; };
template <> struct is_integer<long> { static const bool value = true; };
template <> struct is_integer<unsigned long> { static const bool value = true; };
template <> struct is_integer<const unsigned long> { static const bool value = true; };
template <> struct is_integer<long long> { static const bool value = true; };
template <> struct is_integer<unsigned long long> { static const bool value = true; };
template <> struct is_integer<const unsigned long long> { static const bool value = true; };

template <typename T> struct is_unsigned{ static const bool value = false; };
template <> struct is_unsigned<bool> { static const bool value = false; };
template <> struct is_unsigned<const bool> { static const bool value = false; };
template <> struct is_unsigned<char> { static const bool value = false; };
template <> struct is_unsigned<const char> { static const bool value = false; };
template <> struct is_unsigned<unsigned char> { static const bool value = true; };
template <> struct is_unsigned<const unsigned char> { static const bool value = true; };
template <> struct is_unsigned<short> { static const bool value = false; };
template <> struct is_unsigned<const short> { static const bool value = false; };
template <> struct is_unsigned<unsigned short> { static const bool value = true; };
template <> struct is_unsigned<const unsigned short> { static const bool value = true; };
template <> struct is_unsigned<int> { static const bool value = false; };
template <> struct is_unsigned<const int> { static const bool value = false; };
template <> struct is_unsigned<unsigned int> { static const bool value = true; };
template <> struct is_unsigned<const unsigned int> { static const bool value = true; };
template <> struct is_unsigned<long> { static const bool value = false; };
template <> struct is_unsigned<unsigned long> { static const bool value = true; };
template <> struct is_unsigned<const unsigned long> { static const bool value = true; };
template <> struct is_unsigned<long long> { static const bool value = false; };
template <> struct is_unsigned<unsigned long long> { static const bool value = true; };
template <> struct is_unsigned<const unsigned long long> { static const bool value = true; };

template <typename T> struct is_float { static const bool value = false; };
template <> struct is_float<float> { static const bool value = true; };
template <> struct is_float<double> { static const bool value = true; };
template <> struct is_float<long double> { static const bool value = true; };

template <typename T> struct is_number
{
    static const bool value = is_integer<T>::value || is_unsigned<T>::value || is_float<T>::value;
};

template <typename T> struct is_str { static const bool value = false; };
template <> struct is_str<char*> { static const bool value = true; };
template <> struct is_str<const char*> { static const bool value = true; };

template <typename T> struct is_string { static const bool value = false; };
template <> struct is_string<std::string> { static const bool value = true; };
template <> struct is_string<const std::string> { static const bool value = true; };

template <typename T>
struct is_const_pointer
{
    template <typename U>
    static t_yes is_const_pointer_tester(void (*)(const U*));
    static t_no is_const_pointer_tester(...);

    enum {
        value = (sizeof(t_yes) == sizeof(is_const_pointer_tester((void (*)(T))0)))
    };
};

template <typename T>
struct is_pointer
{
    template <typename U> static t_yes is_pointer_tester(U*);
    static t_no is_pointer_tester(...);

    enum {
        value = (sizeof(t_yes) == sizeof(is_pointer_tester((T)0)))
    };
};

template <typename T>
struct is_const
{
    enum {
        value = (is_pointer<T>::value && \
                is_const_pointer<typename remove_pointer<T>::type*>::value) \
            || is_const_pointer<T*>::value
    };
};

template <typename T>
struct is_class_or_union
{
    template <typename U> static t_yes is_class_or_union_tester(void (U::*)());
    template <typename U> static t_no is_class_or_union_tester(...);

    enum {
        value = (sizeof(t_yes) == sizeof(is_class_or_union_tester<T>(0)))
    };
};

template <typename T, typename U>
struct is_same_type
{
    template <typename X, typename Y>
    struct is_same_type_tester { static const bool value = false; };
    template <typename X>
    struct is_same_type_tester<X, X> { static const bool value = true; };

    enum {
        value = is_same_type_tester<T, U>::value
    };
};

template <typename T, typename U>
struct is_drived_from
{
    static t_yes is_drived_from_tester(U* u);
    static t_no is_drived_from_tester(...);

    enum {
        value = (sizeof(t_yes) == sizeof(is_drived_from_tester((T*)0)))
    };
};

template <typename T, typename U>
struct is_subclass
{
    enum {
        value = is_drived_from<T, U>::value
    };
};
// end for is types

// XXX: avoid cast warnings
template <typename TO, typename FROM>
inline TO nasty_cast(FROM f) {
    union {
        FROM f;
        TO t;
    } u;
    u.f = f;
    return u.t;
}

} // namespace buf

#endif // REMOVETYPES_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

