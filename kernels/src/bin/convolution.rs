
const LIST_LEN: usize = 4;
const KERNEL_LEN: usize = 2;
const RESULT_LEN: usize = LIST_LEN - KERNEL_LEN + 1;

fn do_convolution(list: &[i32; LIST_LEN], kernel: &[i32; KERNEL_LEN], result: &mut [i32; RESULT_LEN]) {
    for i in 0..RESULT_LEN {
        let mut curr_result = 0;
        for j in 0..KERNEL_LEN {
            curr_result += list[i + j] * kernel[j];
        }
        result[i] = curr_result;
    }
}

fn main() {
    let list: [i32; LIST_LEN] = [1, 2, 3, 4];
    let kernel: [i32; KERNEL_LEN] = [1, 2];
    let mut result: [i32; RESULT_LEN] = [0; RESULT_LEN];
    do_convolution(&list, &kernel, &mut result);
    println!("{:?}", result);
}