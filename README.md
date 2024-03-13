# Randall - UCLA CS35L Assignment
Assignment Background:
- Given a gzipped git repository.
- Unpack tgz, clone the repository.
- Repo contains a C program that generates a specified number of random bytes of data.
- Ex. ```./randall 100``` generates 100 bytes of pseudorandom data.

Step 1: refactor the code from the single ```randall.c``` file into separate component ```.c``` files, each with a ```.h``` header

Step 2: add options as follows
- ```-i``` ("input") flag can be given the following arguments:
  - ```rdrand``` (default) instructs the program to use x86-64 hardware randomness as the source of the input (if supported by CPU)
    - both the implementation and the support-checking are already in the given code
  - ```lrand48_r``` instructs the program to use the GNU C library ```lrand48_r``` randomness function as the source of the input.
    - TAs also allowed use of ```mrand48_r```
  - ```/F``` (where F is a file path) instructs the program to use the given file as the source of the input
- ```-i``` ("output") flag can be given the following arguments:
  - ```stdio``` (default) outputs all random data at once with no modification
  - ```N``` (where N is an integer) outptus the random data in blocks of N bytes

Examples:
- ```./randall 100 -i rdrand```
- ```./randall 100 -o 10```
- ```./randall 100 -i /dev/urandom -o stdio```
