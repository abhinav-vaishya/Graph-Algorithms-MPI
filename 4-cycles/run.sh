#!/bin/bash

echo "compiling..."
mpic++ parallel.cpp -std=c++11
echo "done!"
echo 
echo "executing..."
mpirun -np 2 a.out testcases/1.in
