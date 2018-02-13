#!/bin/bash
#parallel area
for k in omni icc; do
for j in 2 4; do
export OMP_NUM_THREADS=$j
for ((i=50;i<=1500;i=i+50));do
./gs-$k $i 0.0000001 $j
done;
mv time-par.dat ./time-gs-omp-$k-$j.dat
done;
done;
echo "end"
