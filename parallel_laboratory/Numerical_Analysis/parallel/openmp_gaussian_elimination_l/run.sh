#!/bin/bash
for k in icc omni; do
for j in 2; do
export OMP_NUM_THREADS=$j
#./gauss-$k 50 $j
#for ((i=250;i<=2500;i=i+250));do
for ((i=50;i<=2500;i=i+50));do
./gauss-$k $i $j
done;
#mv time-par.dat time-par-gauss-omp-$k-$j-2500-250.dat
mv time-par.dat time-par-gauss-omp-$k-$j-2500.dat
done;
done;
echo "end openMP"
