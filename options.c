#include <stdbool.h> // so we can use "true" and "false" for booleans instead of 1 and 0.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "options.h"

void parse_option(int argc, char **argv, struct options* options) {
    options->is_valid = false;
    int c;
    /* use getopt to parse each -i and then each -o option 
    https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html */
    while ((c = getopt(argc, argv, ":i:o:")) != -1) {
        switch(c) {
            case 'i':
                /* strcmp must be used instead of == because C */
                if (strcmp("rdrand", optarg) == 0) {
                    options->input_opts = RDRAND;
                }
                else if (strcmp("lrand48_r", optarg) == 0) {
                    options->input_opts = LRAND48_R;
                }
                else if (optarg[0] == '/') {
                    options->input_opts = FILE_INPUT;
                    options->rand_src_filename = optarg;
                }
                else {
                    return;
                }
                options->is_valid = true;
                break;
            case 'o':
                if (strcmp("stdout", optarg) == 0) {
                    options->output_opts = STDIO;
                }
                else {
                    options->output_opts = N;
                    options->output_block_size = atoi(optarg);
                    if (options->output_block_size == 0) {
                        fprintf(stderr, "0 is not a valid output block size");
                        return;
                    }
                }
                options->is_valid = true;
                break;
            /* catch error for no operand */
            case ':':
                fprintf(stderr, "Options requiring an operand were not supplied with one.");
                return;
            /* catch unsupported options */
            case '?':
                fprintf(stderr, "Unsupported options.");
                return;
        }
    }
    if (optind >= argc) {
        return;
    }
    options->num_input_bytes = atol(argv[optind]);
    if (options->num_input_bytes >= 0) {
        options->is_valid = true;
    }
}