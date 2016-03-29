#!/bin/sh

c++ -g -I../sexpresso -I../sexpresso_std -o test-sexpresso-std '-std=c++11' test_sexpresso_std.cpp ../sexpresso/sexpresso.cpp ../sexpresso_std/sexpresso_std.cpp
./test-sexpresso-std $*
rm ./test-sexpresso-std
