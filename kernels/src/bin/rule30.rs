#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}


const LENGTH: usize = 32;
const STEPS: usize = 16;

#[no_mangle]
#[inline(never)]
pub extern "C" fn ca_step_1d(
    cur: & [i32; LENGTH],
    next: &mut [i32; LENGTH],
    length: usize,
) {
    let mut i: usize = 1;
    while i < length - 1 {
        unsafe { please_map_me() };
        let left   = cur[i - 1];
        let center = cur[i];
        let right  = cur[i + 1];

        /* Rule 30 */
        next[i] = left ^ (center | right);

        i += 1;
    }
}

#[no_mangle]
fn rust_entry() {

    let mut cur: [i32; LENGTH] = [0; LENGTH];
    let mut next: [i32; LENGTH] = [0; LENGTH];

    /* Initialize state */
    cur[LENGTH / 2] = 1;

    let mut step: usize = 0;
    while step < STEPS {

        /* Print current state */
        let mut i: usize = 0;
        while i < LENGTH {
            if cur[i] != 0 {
                print!("#");
            } else {
                print!(".");
            }
            i += 1;
        }
        println!();

        /* Apply kernel */
        ca_step_1d(&cur, &mut next, LENGTH);

        /* Swap buffers */
        let mut j: usize = 0;
        while j < LENGTH {
            cur[j] = next[j];
            next[j] = 0;
            j += 1;
        }

        step += 1;
    }
}


fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
