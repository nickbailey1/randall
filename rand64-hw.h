_Bool rdrand_supported(void);

void hardware_rand64_init(void);

unsigned long long hardware_rand64();

unsigned long long hardware_lrand48();

void hardware_rand64_fini(void);