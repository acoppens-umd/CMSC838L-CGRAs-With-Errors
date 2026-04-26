#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}

const MAX_PAYLOAD: usize = 16;
const NUM_RECORDS: usize = 4;

// Accumulates the payload bytes of one record. `length` comes from the packet
// header at runtime -- the bounds check on payload[i] fires if the header is
// malformed (length > MAX_PAYLOAD), which would be silent memory corruption in C.
#[no_mangle]
#[inline(never)]
pub extern "C" fn parse_record(payload: &[i32; MAX_PAYLOAD], length: usize) -> i32 {
    let mut sum = 0;
    for i in 0..length {
        unsafe { please_map_me() };
        sum += payload[i];
    }
    sum
}

#[no_mangle]
fn rust_entry() {
    // Payload bytes for each record, padded to MAX_PAYLOAD
    let records: [[i32; MAX_PAYLOAD]; NUM_RECORDS] = [
        [ 1,  2,  3,  4,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 5,  6,  7,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 8,  9, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [13, 14,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    ];
    // Lengths are read from packet headers at runtime
    let lengths: [usize; NUM_RECORDS] = [4, 3, 5, 2];

    let mut r: usize = 0;
    while r < NUM_RECORDS {
        let result = parse_record(&records[r], lengths[r]);
        println!("record {}: sum = {}", r, result);
        r += 1;
    }
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
