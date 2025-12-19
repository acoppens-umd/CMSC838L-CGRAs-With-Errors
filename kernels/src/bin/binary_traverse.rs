#![allow(dead_code)]

extern "C" {
    fn please_map_me() -> i32; // no definition provided
}


// NUM_ELEMENTS_IN_LIST should be 2^(MAX_LEVEL + 1) - 1
// This program only does traversal on perfect binary trees
const MAX_LEVEL: i32 = 3;
const NUM_ELEMENTS_IN_LIST: usize = 15;
const NUM_ELEMENTS_IN_LIST_i32: i32 = NUM_ELEMENTS_IN_LIST as i32;

// Returns an inorder traversal of the list
#[no_mangle]
#[inline(never)]
pub extern "C" fn binary_traverse(tree_list: &[i32; NUM_ELEMENTS_IN_LIST], traversal: &mut[i32; NUM_ELEMENTS_IN_LIST]) {
    let mut curr_index: i32 = 0;
    let mut prev_index: i32 = NUM_ELEMENTS_IN_LIST_i32;
    let mut curr_level: i32 = 0;
    let mut curr_traversal_index: i32 = 0;

    while curr_level <= MAX_LEVEL {
        unsafe { please_map_me() };
        let parent_index: i32;
        if curr_index == 0 {
            parent_index = NUM_ELEMENTS_IN_LIST_i32;
        }
        else {
            parent_index = (curr_index - 1) >> 1;
        }
        let left_index: i32 = (curr_index * 2) + 1;
        let right_index: i32 = (curr_index * 2) + 2;
        if prev_index == parent_index || prev_index >= NUM_ELEMENTS_IN_LIST_i32 {
            if curr_level == MAX_LEVEL {  
                traversal[curr_traversal_index as usize] = tree_list[curr_index as usize];
                prev_index = curr_index;
                curr_index = parent_index;
                curr_traversal_index += 1;
                if curr_level > 0 {
                    curr_level -= 1;
                }
                else {
                    curr_level = MAX_LEVEL + 1;
                }
            }
            else {
                prev_index = curr_index;
                curr_index = left_index;
                curr_level += 1;
            }
        }
        else if prev_index == left_index {
            traversal[curr_traversal_index as usize] = tree_list[curr_index as usize];
            prev_index = curr_index;
            curr_index = right_index;
            curr_traversal_index += 1;
            curr_level += 1;
        }
        else {
            prev_index = curr_index;
            curr_index = parent_index;
            if curr_level > 0 {
                curr_level -= 1;
            }
            else {
                curr_level = MAX_LEVEL + 1;
            }
        }
    }

}

#[no_mangle]
fn rust_entry() {
    let tree_list: [i32; NUM_ELEMENTS_IN_LIST] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    let mut result_list: [i32; NUM_ELEMENTS_IN_LIST] =[0; NUM_ELEMENTS_IN_LIST];
    binary_traverse(&tree_list, &mut result_list);
    println!("{:?}", result_list);
}

fn main() {
    // Rust runtime entry
    let _ = rust_entry();
}
