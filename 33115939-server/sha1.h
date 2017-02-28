// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

#ifndef SHA1_H
#define SHA1_H

#if _WIN32
typedef unsigned long long u_int64_t;
#endif

#include <stdlib.h>
#include <string.h>

unsigned char *
SHA1(const unsigned char *d, size_t n, unsigned char *md);

#endif /* SHA1_H */
