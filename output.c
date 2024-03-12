#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "output.h"

bool print_output_bytes(unsigned long long rand_output_int, int num_output_bytes) {
    /* need a do-while to ensure last byte is output, can't use while loop w/ condition
    num_output_bytes > 0 because then putchar will error */
    do {
        /* write rand_output_int to stdout 
        https://www.gnu.org/software/libc/manual/html_node/Simple-Output.html */
        if (putchar(rand_output_int) < 0) {
            return false;
        }
        rand_output_int = rand_output_int >> CHAR_BIT;
        num_output_bytes--;
    }
    while (num_output_bytes > 0);
    return true;
}