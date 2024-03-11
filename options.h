enum Input { BLANK, RDRAND, LRAND48_R, SLASH_F}
enum Output { STDOUT, N }

struct options {
    bool isValid; // true if all opts are valid, false if not
    long long nbytes;
    enum Input input;
    char* r_src;
    enum Output output;
    unsigned int block_size;
};

void readopt(int argc, char **argv, struct options* options);