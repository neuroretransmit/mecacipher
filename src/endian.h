#pragma once

#include <climits>
#include <cstdint>

using namespace std;

/**
 * @brief check architecture endianness
 * @return is big endian
 */
inline bool is_big_endian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

/**
 * @brief swap endianness of value
 * @tparam T type of value
 * @param u value
 */
template<typename T> T swap_endian(T u)
{
    static_assert(CHAR_BIT == 8, "swap_endian(): CHAR_BIT != 8");

    union {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}
