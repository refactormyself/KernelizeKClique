#!/usr/bin/env bash
set -e

rm -r ../build/
mkdir -p ../build/

# load graph from file
nvcc -g ../src/edge-dom_cu.cu  -o ../build/edgedom \
--run   
# --run-args 125,5500
# --run-args 6,7,'../data/graph_6_7.dat'

# generate random graphs
# nvcc ../src/edge-dom_cu.cu  -o ../build/edgedom --run

# cuda-gdb --args ../build/edgedom 125 5500


# OLD ----
# g++ -std=c++14 -Wall -Wextra -O3 ../src/edge-dom.cpp  -o ../build/edgedom 

# echo "Running Application ..."
# ../build/edgedom 125 ../data/graph_125_5500.dat
