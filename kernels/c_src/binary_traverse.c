
//#include <stdio.h>

// NUM_ELEMENTS_IN_LIST should be 2^(MAX_LEVEL + 1) - 1
// Only works with perfect binary trees
#define MAX_LEVEL 3
#define NUM_ELEMENTS_IN_LIST 15

// Returns an inorder traversal of the list representing a tree
__attribute__((noinline))
void binary_traverse(int tree_list[NUM_ELEMENTS_IN_LIST], int traversal[NUM_ELEMENTS_IN_LIST]) {
    unsigned int curr_index = 0;
    unsigned int prev_index = NUM_ELEMENTS_IN_LIST;
    unsigned int curr_level = 0;
    unsigned int curr_traversal_index = 0;

    while (curr_level <= MAX_LEVEL) {
        #ifdef CGRA_COMPILER
        please_map_me();
        #endif
        unsigned int parent_index;
        if (curr_index == 0) {
            parent_index = NUM_ELEMENTS_IN_LIST;
        }
        else {
            parent_index = (curr_index - 1) >> 1;
        }
        unsigned int left_index = (curr_index * 2) + 1;
        unsigned int right_index = (curr_index * 2) + 2;
        if (prev_index == parent_index || prev_index >= NUM_ELEMENTS_IN_LIST) {
            if (curr_level == MAX_LEVEL) {  
                traversal[curr_traversal_index] = tree_list[curr_index];
                prev_index = curr_index;
                curr_index = parent_index;
                curr_traversal_index += 1;
                if (curr_level > 0) {
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
        else if (prev_index == left_index) {
            traversal[curr_traversal_index] = tree_list[curr_index];
            prev_index = curr_index;
            curr_index = right_index;
            curr_traversal_index += 1;
            curr_level += 1;
        }
        else {
            prev_index = curr_index;
            curr_index = parent_index;
            if (curr_level > 0) {
                curr_level -= 1;
            }
            else {
                curr_level = MAX_LEVEL + 1;
            }
        }
    }

}

int main() {
    int tree_list[NUM_ELEMENTS_IN_LIST] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int result_list[NUM_ELEMENTS_IN_LIST] = {0};
    binary_traverse(tree_list, result_list);
    /*for (int i = 0; i < NUM_ELEMENTS_IN_LIST; i++) {
        printf("%d ", result_list[i]);
    }
    printf("\n");*/
}