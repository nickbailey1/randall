#!/bin/bash

# tests different options to make sure all are read correctly - would see errors in stderr if not
# can't really check the actual output, so just verify that 100 bytes are output each time
./randall 100 -i lrand48_r | wc -c
./randall 100 -i /dev/full | wc -c
./randall 100 -i rdrand | wc -c
./randall 100 | wc -c

./randall 100 -o 10 | wc -c
./randall 100 -o stdio | wc -c
