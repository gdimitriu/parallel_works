#!/bin/bash
for k in gcc icc; do
for j in 2 4; do
export OMP_NUM_THREADS=$j
for ((i=50;i<=2500;i=i+50));do
./mul-$k $i $j
done;
mv time-par.dat time-par-mul-omp-$k-$j.dat
done;
done;
echo "end openMP mul 1"
