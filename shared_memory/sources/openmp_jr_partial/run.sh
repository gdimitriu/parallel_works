#!/bin/bash
#parallel area
for k in omni icc; do
for j in 2; do
export OMP_NUM_THREADS=$j
for ((i=50;i<=2500;i=i+50));do
./jacobi-$k $i 0.0000001 $j
done;
mv time-par.dat ./time-jacobi-row-omp-$k-$j-2500.dat
done;
done;
echo "end"
