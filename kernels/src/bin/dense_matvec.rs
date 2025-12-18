
const M: usize = 3;
const K: usize = 2;

fn densematvec(matrix: &[[i32; K]; M], vector: &[i32; K], out: &mut[i32; M]) {
    for curr_m in 0..M {
        for curr_k in 0..K {
            out[curr_m] += matrix[curr_m][curr_k] * vector[curr_k];
        }
    }
}

fn main() {
    let matrix: [[i32; K]; M] = [[1,2],[3,4],[5,6]];
    let vector: [i32; K] = [2,3];
    let mut out: [i32; M] = [0; M];
    densematvec(&matrix, &vector, &mut out);
    println!("{:?}", out);
}