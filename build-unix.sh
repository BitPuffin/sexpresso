#!/bin/sh
c++ -pedantic -O3 '-std=c++11' -c sexpresso/sexpresso.cpp
ar rcs libsexpresso.a sexpresso.o
