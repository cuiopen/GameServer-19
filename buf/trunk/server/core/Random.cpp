
#include "Random.h"
#include "Config.h"

#include <time.h> // for time
#ifdef __GNUC__
#if (defined(USE_RANDOM) && USE_RANDOM)\
    || (defined(USE_URANDOM) && USE_URANDOM)
#include <fcntl.h> // for open
#include <unistd.h> // for close
#endif
#include <stdlib.h> // for srand,rand_r,srandom,random
#else
#error "No implementation without __GNUC__"
#endif

namespace buf
{

int rand_value(void);
int rand_between(int s, int e);

int Random::operator()()
{
    return rand_value();
}

int Random::operator()(int max)
{
    return rand_between(0, max);
}

int Random::operator()(int min, int max)
{
    return rand_between(min, max);
}


static bool random_inited = false;

#if defined(USE_RAMDOM) && USE_RAMDOM
static int random_fd = -1;
#endif

#if defined(USE_URANDOM) && USE_URANDOM
static int urandom_fd = -1;
#endif

void rand_init(void)
{
    if (!random_inited)
    {
        int value = 0;
        UNUSE(value);
#ifdef __GNUC__
#   if defined(USE_RANDOM) && USE_RANDOM
        if (random_fd < 0)
            random_fd = open("/dev/random", O_RDONLY);

        if (read(random_fd, &value, sizeof(value)) == sizeof(value))
        {
            srand(value);
            //srandom(value);
        }
#   elif defined(USE_URANDOM) && USE_URANDOM
        if (urandom_fd < 0)
            urandom_fd = open("/dev/urandom", O_RDONLY);

        if (read(urandom_fd, &value, sizeof(value)) == sizeof(value))
        {
            srand(value);
        }
#   endif
#endif

        static unsigned int random_seed = 0;
        random_seed = value?value:time(0);
        srand(random_seed);
        random_inited = true;
    }
}

void rand_destroy(void)
{
    if (random_inited)
    {
#ifdef __GNUC__
#   if defined(USE_RANDOM) && USE_RANDOM
        if (random_fd > 0)
            close(random_fd);
#   elif defined(USE_URANDOM) && USE_URANDOM
        if (urandom_fd > 0)
            close(urandom_fd);
#   endif
#endif
        random_inited = false;
    }
}

int rand_between(int s, int e)
{
    if (s == e)
        return s;

    if (!random_inited)
        rand_init();

    if (s < e)
    {
        int seed = e - s;
        return s + (static_cast<unsigned int>((static_cast<double>(seed) * rand_value())) % (seed + 1));
    }
    else
    {
        int seed = s - e;
        return e + (static_cast<unsigned int>((static_cast<double>(seed) * rand_value())) % (seed + 1));
    }

    return 0;
}

int rand_value(void)
{
    if (!random_inited)
        rand_init();

#ifdef __GNUC__
    int value = 0;
    UNUSE(value);
#   if defined(USE_RANDOM) && USE_RANDOM
    if (random_fd > 0
            && read(random_fd, &value, sizeof(value)) == sizeof(value))
        return value;
#   elif defined(USE_URANDOM) && USE_URANDOM
    if (urandom_fd > 0
            && read(urandom_fd, &value, sizeof(value)) == sizeof(value))
        return value;
#   endif
#endif
    return rand();
}

} // namespace buf

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

