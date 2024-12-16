#ifndef _LOADER_H
#define _LOADER_H

#include <stddef.h>

size_t strlen(const char *);
size_t strnlen(const char *, size_t);
void *memset(void *, int, size_t);
void *memcpy(void *, const void *, size_t);
int memcmp(const void *, const void *, size_t);

typedef signed char s8;
typedef unsigned char u8;
typedef signed short int s16;
typedef unsigned short int u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long int s64;
typedef unsigned long long int u64;

static inline u16 le16(const u8 *p)
{
    return p[0] | (p[1] << 8);
}

static inline u32 le32(const u8 *p)
{
    return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static inline u32 read32(u32 addr)
{
    u32 x;
    asm volatile("lwz %0,0(%1) ; sync" : "=r"(x) : "b"(0xc0000000 | addr));
    return x;
}

static inline void write32(u32 addr, u32 x)
{
    asm("stw %0,0(%1) ; eieio" : : "r"(x), "b"(0xc0000000 | addr));
}

static inline u16 read16(u32 addr)
{
    u16 x;
    asm volatile("lhz %0,0(%1) ; sync" : "=r"(x) : "b"(0xc0000000 | addr));
    return x;
}

static inline void write16(u32 addr, u16 x)
{
    asm("sth %0,0(%1) ; eieio" : : "r"(x), "b"(0xc0000000 | addr));
}

static inline u32 virt_to_phys(const void *p)
{
    return (u32)p & 0x7fffffff;
}

static inline void *phys_to_virt(u32 x)
{
    return (void *)(x | 0x80000000);
}

void sync_before_read(void *p, u32 len);
void sync_after_write(const void *p, u32 len);
void sync_before_exec(const void *p, u32 len);

void udelay(u32 us);

#define mtspr(n, x) do { asm("mtspr %1,%0" : : "r"(x), "i"(n)); } while (0)

#define mfspr(n) ({ \
    u32 x; \
    asm volatile("mfspr %0,%1" : "=r"(x) : "i"(n)); \
    x; \
})

void exception_init(void);

extern const char version[];

void video_init(void);
void fb_putc(char);

void console_init(void);
int printf(const char *fmt, ...);

int valid_elf_image(void *addr);
void *load_elf_image(void *addr);

struct ioctlv {
    void *data;
    u32 len;
};

int ios_open(const char *filename, u32 mode);
int ios_close(int fd);
int ios_read(int fd, void *data, u32 len);
int ios_write(int fd, const void *data, u32 len);
int ios_seek(int fd, int where, int whence);
int ios_ioctl(int fd, u32 n, const void *in, u32 inlen, void *out, u32 outlen);
int ios_ioctlv(int fd, u32 n, u32 in_count, u32 out_count, struct ioctlv *vec);

void reset_ios(void);

enum {
    TINFL_FLAG_PARSE_ZLIB_HEADER = 1,
    TINFL_FLAG_HAS_MORE_INPUT = 2,
    TINFL_FLAG_USING_NON_WRAPPING_OUTPUT_BUF = 4,
    TINFL_FLAG_COMPUTE_ADLER32 = 8
};

#define TINFL_DECOMPRESS_MEM_TO_MEM_FAILED ((size_t)(-1))

size_t tinfl_decompress_mem_to_mem(void *pOut_buf, size_t out_buf_len, const void *pSrc_buf, size_t src_buf_len, int flags);

#endif
