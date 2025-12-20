
//#include <stdio.h>

#define WIDTH   8
#define HEIGHT  8

__attribute__((noinline))
void edge_detect_int(
    const int *in,
    int *out,
    int width,
    int height
) {
    int y, x;

    for (y = 1; y < height - 1; y++) {
        for (x = 1; x < width - 1; x++) {
            #ifdef CGRA_COMPILER
            please_map_me();
            #endif

            int idx = y * width + x;

            /* Load 3x3 neighborhood */
            int a = in[idx - width - 1];
            int b = in[idx - width];
            int c = in[idx - width + 1];

            int d = in[idx - 1];
            int e = in[idx];
            int f = in[idx + 1];

            int g = in[idx + width - 1];
            int h = in[idx + width];
            int i = in[idx + width + 1];

            /* Prewitt-like gradients (no division) */
            int gx = (c + f + i) - (a + d + g);
            int gy = (g + h + i) - (a + b + c);

            /* Absolute value without abs() */
            int maskx = gx >> 31;
            int masky = gy >> 31;

            gx = (gx ^ maskx) - maskx;
            gy = (gy ^ masky) - masky;

            /* Edge magnitude (L1 norm) */
            out[idx] = gx + gy;
        }
    }
}

int main(void)
{
    int x, y;

    /* Input and output images */
    int input[WIDTH * HEIGHT];
    int output[WIDTH * HEIGHT];

    /* Initialize input image with a simple pattern */
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            input[y * WIDTH + x] = (x + y) & 0xF;
        }
    }

    /* Clear output buffer */
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            output[y * WIDTH + x] = 0;
        }
    }

    /* Run edge detection kernel */
    edge_detect_int(input, output, WIDTH, HEIGHT);

    /* Optional: print result for verification */
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            printf("%4d ", output[y * WIDTH + x]);
        }
        printf("\n");
    }

    return 0;
}