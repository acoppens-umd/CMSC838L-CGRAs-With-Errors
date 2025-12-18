
// NUM_ELEMENTS_IN_LIST should be 2^(MAX_LEVEL + 1) - 1
// This program only does traversal on perfect binary trees
const MAX_LEVEL: usize = 3;
const NUM_ELEMENTS_IN_LIST: usize = 15;

// Returns an inorder traversal of the list
fn binary_traverse(tree_list: &[i32; NUM_ELEMENTS_IN_LIST], traversal: &mut[i32; NUM_ELEMENTS_IN_LIST]) {
    let mut curr_index: usize = 0;
    let mut prev_index: usize = NUM_ELEMENTS_IN_LIST;
    let mut curr_level: usize = 0;
    let mut curr_traversal_index: usize = 0;

    while curr_level <= MAX_LEVEL {
        let parent_index: usize;
        if curr_index == 0 {
            parent_index = NUM_ELEMENTS_IN_LIST;
        }
        else {
            parent_index = (curr_index - 1) >> 1;
        }
        let left_index: usize = (curr_index * 2) + 1;
        let right_index: usize = (curr_index * 2) + 2;
        if prev_index == parent_index || prev_index >= NUM_ELEMENTS_IN_LIST {
            if curr_level == MAX_LEVEL {  
                traversal[curr_traversal_index] = tree_list[curr_index];
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
            traversal[curr_traversal_index] = tree_list[curr_index];
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

fn main() {
    let tree_list: [i32; NUM_ELEMENTS_IN_LIST] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    let mut result_list: [i32; NUM_ELEMENTS_IN_LIST] =[0; NUM_ELEMENTS_IN_LIST];
    binary_traverse(&tree_list, &mut result_list);
    println!("{:?}", result_list);
}