#!/usr/bin/env bash
set -e

rm -r ../build/
mkdir -p ../build/
#nvcc -std=c++11 -O3 -g ../src/edge-dom.cu  -o ../build/edgedom-thrust 
nvcc -std=c++11 -g ../src/edge-dom_cu.cpp  -o ../build/edgedom-thrust 

echo "Running Application ..."
#../build/edgedom-thrust 125 5500 ../data/graph_125_5500.dat
../build/edgedom-thrust 6 7 ../data/data_6.dat
#../build/edgedom-thrust 125 5500 
#../build/edgedom-thrust 255 5500 
#../build/edgedom-thrust 6 7 

echo
echo NOW Profiling ....
echo

#nvprof --print-gpu-trace ../build/edgedom-thrust 125 ../data/graph_125_5500.dat # --benchmark -numdevices=2 -i=1
