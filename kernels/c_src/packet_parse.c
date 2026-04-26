
#define MAX_PAYLOAD 16
#define NUM_RECORDS 4

/* Accumulates the payload bytes of one record. `length` comes from the packet
   header at runtime -- no bounds check in C, so a malformed header silently
   reads past the end of payload (buffer over-read). */
__attribute__((noinline))
int parse_record(int payload[MAX_PAYLOAD], unsigned int length) {
    int sum = 0;
    unsigned int i;
    for (i = 0; i < length; i++) {
        #ifdef CGRA_COMPILER
        please_map_me();
        #endif
        sum += payload[i];
    }
    return sum;
}

int main() {
    /* Payload bytes for each record, padded to MAX_PAYLOAD */
    int records[NUM_RECORDS][MAX_PAYLOAD] = {
        { 1,  2,  3,  4,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 5,  6,  7,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 8,  9, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {13, 14,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
    /* Lengths are read from packet headers at runtime */
    unsigned int lengths[NUM_RECORDS] = {4, 3, 5, 2};

    unsigned int r;
    for (r = 0; r < NUM_RECORDS; r++) {
        parse_record(records[r], lengths[r]);
    }
}
