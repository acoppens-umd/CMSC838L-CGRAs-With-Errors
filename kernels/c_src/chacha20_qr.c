
#define N 4

#define ROTL32(x, n) (((unsigned int)(x) << (n)) | ((unsigned int)(x) >> (32 - (n))))

__attribute__((noinline))
void chacha20_quarter_round(
    unsigned int a[N],
    unsigned int b[N],
    unsigned int c[N],
    unsigned int d[N]
) {
    unsigned int i;
    for (i = 0; i < N; i++) {
        #ifdef CGRA_COMPILER
        please_map_me();
        #endif
        unsigned int av = a[i], bv = b[i], cv = c[i], dv = d[i];

        av += bv; dv ^= av; dv = ROTL32(dv, 16);
        cv += dv; bv ^= cv; bv = ROTL32(bv, 12);
        av += bv; dv ^= av; dv = ROTL32(dv,  8);
        cv += dv; bv ^= cv; bv = ROTL32(bv,  7);

        a[i] = av; b[i] = bv; c[i] = cv; d[i] = dv;
    }
}

int main() {
    /* RFC 8439 §2.1.1 test vector, replicated across N independent inputs */
    unsigned int a[N] = {0x11111111, 0x11111111, 0x11111111, 0x11111111};
    unsigned int b[N] = {0x01020304, 0x01020304, 0x01020304, 0x01020304};
    unsigned int c[N] = {0x9b8d6f43, 0x9b8d6f43, 0x9b8d6f43, 0x9b8d6f43};
    unsigned int d[N] = {0x01234567, 0x01234567, 0x01234567, 0x01234567};
    chacha20_quarter_round(a, b, c, d);
}
