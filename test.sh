#!/bin/bash

./randall 100 -i lrand48_r | wc -c
./randall 100 -i /dev/full | wc -c
./randall 100 -i rdrand | wc -c
./randall 100 | wc -c

./randall 100 -o 10 | wc -c
./randall 100 -o stdio | wc -c
