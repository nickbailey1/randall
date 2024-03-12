enum Input { NONE, RDRAND, LRAND48_R, FILE} // enumerate options regarding input
enum Output { STDIO, N } // enumerate options regarding output

struct options {
    bool is_valid; // true if all options are valid, false if not
    long long num_input_bytes;
    enum Input input;
    char* rand_src_filename;
    enum Output output;
    unsigned int output_block_size;
};

void read_options(int argc, char **argv, struct options* options);