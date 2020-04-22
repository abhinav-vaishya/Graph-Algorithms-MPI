#!/bin/bash

echo "compiling..."
mpic++ main.cpp auxiliary.cpp -o triangles -std=c++11
echo "done!"
echo 
echo "executing..."
mpirun -np 5 ./triangles 
