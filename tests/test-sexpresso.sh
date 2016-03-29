#!/bin/sh

c++ -g -I../sexpresso -o test-sexpresso '-std=c++11' test_sexpresso.cpp ../sexpresso/sexpresso.cpp
./test-sexpresso $*
rm ./test-sexpresso
