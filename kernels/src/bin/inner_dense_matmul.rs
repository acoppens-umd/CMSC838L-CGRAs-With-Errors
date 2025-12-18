
const M: usize = 4;
const K: usize = 5;
const N: usize = 6;


// Conducts inner product matrix multiplication with dense matrices
fn innermatmul(a: &[[i32; K]; M], b: &[[i32; N]; K], out: &mut[[i32; N]; M]) {
    for curr_m in 0..M {
        for curr_n in 0..N {
            let mut curr_entry: i32 = 0;
            for curr_k in 0..K {
                curr_entry += a[curr_m][curr_k] * b[curr_k][curr_n];
            }
            out[curr_m][curr_n] = curr_entry;
        }
    }
}

fn main() {
    let a: [[i32; K]; M] = [[1,2,3,4,5],[6,7,8,9,10],[11,12,13,14,15],[16,17,18,19,20]];
    let b: [[i32; N]; K] = [[1,2,3,4,5,6],[7,8,9,10,11,12],[13,14,15,16,17,18],[19,20,21,22,23,24],[25,26,27,28,29,30]];
    let mut out: [[i32; N]; M] = [[0; 6]; 4];
    innermatmul(&a, &b, &mut out);
    println!("{:?}", out)
}

