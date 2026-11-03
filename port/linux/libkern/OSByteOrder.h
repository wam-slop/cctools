#ifndef LD64_COMPAT_OSBYTEORDER_H
#define LD64_COMPAT_OSBYTEORDER_H

#include <stdint.h>
#include <string.h>

static inline uint16_t OSSwapInt16(uint16_t v) { return __builtin_bswap16(v); }
static inline uint32_t OSSwapInt32(uint32_t v) { return __builtin_bswap32(v); }
static inline uint64_t OSSwapInt64(uint64_t v) { return __builtin_bswap64(v); }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define OSSwapBigToHostInt16(x) OSSwapInt16(x)
#define OSSwapBigToHostInt32(x) OSSwapInt32(x)
#define OSSwapBigToHostInt64(x) OSSwapInt64(x)
#define OSSwapHostToBigInt16(x) OSSwapInt16(x)
#define OSSwapHostToBigInt32(x) OSSwapInt32(x)
#define OSSwapHostToBigInt64(x) OSSwapInt64(x)
#define OSSwapLittleToHostInt16(x) ((uint16_t)(x))
#define OSSwapLittleToHostInt32(x) ((uint32_t)(x))
#define OSSwapLittleToHostInt64(x) ((uint64_t)(x))
#define OSSwapHostToLittleInt16(x) ((uint16_t)(x))
#define OSSwapHostToLittleInt32(x) ((uint32_t)(x))
#define OSSwapHostToLittleInt64(x) ((uint64_t)(x))
#else
#error ld64 compat layer assumes a little-endian host
#endif

#define LD64_OS_READ(width, order)                                                          \
    static inline uint##width##_t OSRead##order##Int##width(const volatile void *base,      \
                                                             uintptr_t offset)              \
    {                                                                                       \
        uint##width##_t v;                                                                  \
        memcpy(&v, (const char *)base + offset, sizeof(v));                                 \
        return OSSwap##order##ToHostInt##width(v);                                          \
    }                                                                                       \
    static inline void OSWrite##order##Int##width(volatile void *base, uintptr_t offset,   \
                                                  uint##width##_t value)                    \
    {                                                                                       \
        uint##width##_t v = OSSwapHostTo##order##Int##width(value);                         \
        memcpy((char *)base + offset, &v, sizeof(v));                                       \
    }

LD64_OS_READ(16, Big)
LD64_OS_READ(32, Big)
LD64_OS_READ(64, Big)
LD64_OS_READ(16, Little)
LD64_OS_READ(32, Little)
LD64_OS_READ(64, Little)

#undef LD64_OS_READ

#endif
