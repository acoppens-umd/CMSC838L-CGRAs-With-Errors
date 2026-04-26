
#define LENGTH 16

__attribute__((noinline))
void stencil_1d(int a[LENGTH], int out[LENGTH]) {
    unsigned int i;
    for (i = 0; i < LENGTH; i++) {
        #ifdef CGRA_COMPILER
        please_map_me();
        #endif
        int left   = (i > 0)            ? a[i - 1] : 0;
        int center = a[i];
        int right  = (i + 1 < LENGTH)   ? a[i + 1] : 0;
        out[i] = left + 2 * center + right;
    }
}

int main() {
    int a[LENGTH]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int out[LENGTH] = {0};
    stencil_1d(a, out);
}
