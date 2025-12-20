#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const WIDTH: usize = 8;
const HEIGHT: usize = 8;

#[no_mangle]         // avoid Rust symbol-mangling (makes names obvious in IR)
#[inline(never)]     // prevent the compiler from inlining this function away

pub extern "C" fn edge_detect_int(
    input: &[i32;  WIDTH * HEIGHT],
    output: &mut [i32;  WIDTH * HEIGHT],
) {

    let mut y: usize = 1;
    while y < HEIGHT - 1 {
        let mut x: usize = 1;
        while x < WIDTH - 1 {
            unsafe { please_map_me() };
            let idx = y * WIDTH + x;

            let a = input[idx - WIDTH - 1];
            let b = input[idx - WIDTH];
            let c = input[idx - WIDTH + 1];

            let d = input[idx - 1];
            let e = input[idx];
            let f = input[idx + 1];

            let g = input[idx + WIDTH - 1];
            let h = input[idx + WIDTH];
            let i = input[idx + WIDTH + 1];

            let mut gx = (c + f + i) - (a + d + g);
            let mut gy = (g + h + i) - (a + b + c);

            /* Absolute value via bit manipulation */
            let maskx = gx >> 31;
            let masky = gy >> 31;

            gx = (gx ^ maskx) - maskx;
            gy = (gy ^ masky) - masky;

            /* Edge magnitude (L1 norm) */
            output[idx] = gx + gy;

            x += 1;
        }
        y += 1;
    }
}

#[no_mangle]
pub extern "C" fn rust_entry() {
    let mut input: [i32;  WIDTH * HEIGHT] = [0; WIDTH * HEIGHT];
    let mut output: [i32;  WIDTH * HEIGHT] = [0; WIDTH * HEIGHT];

    /* Initialize input image with a simple pattern */
    let mut y: usize = 0;
    while y < HEIGHT {
        let mut x: usize = 0;
        while x < WIDTH {
            input[y * WIDTH + x] = ((x + y) & 0xF) as i32;
            x += 1;
        }
        y += 1;
    }

    /* Clear output buffer */
    let mut i: usize = 0;
    while i < WIDTH * HEIGHT {
        output[i] = 0;
        i += 1;
    }

    /* Run edge detection kernel */
    edge_detect_int(&input, &mut output);

    /* Print output for verification */
    let mut row: usize = 0;
    while row < HEIGHT {
        let mut col: usize = 0;
        while col < WIDTH {
            print!("{:4} ", output[row * WIDTH + col]);
            col += 1;
        }
        println!();
        row += 1;
    }
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}

