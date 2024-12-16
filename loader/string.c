#include "loader.h"

size_t strlen(const char *s)
{
    size_t len;

    for (len = 0; s[len]; len++)
        ;

    return len;
}

size_t strnlen(const char *s, size_t count)
{
    size_t len;

    for (len = 0; s[len] && len < count; len++)
        ;

    return len;
}

void *memset(void *b, int c, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
        ((unsigned char *)b)[i] = c;

    return b;
}

void *memcpy(void *dst, const void *src, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++)
        ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];

    return dst;
}

int memcmp(const void *b1, const void *b2, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        int diff = ((unsigned char *)b1)[i] - ((unsigned char *)b2)[i];
        if (diff)
            return diff;
    }

    return 0;
}
