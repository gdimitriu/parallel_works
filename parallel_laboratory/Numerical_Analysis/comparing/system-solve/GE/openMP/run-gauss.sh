#!/bin/bash
for k in fast; do
#mkdir $k
#2 4 8 16 24 32 64 128
for j in 2 4 8 16 24 32 64 128; do
export OMP_NUM_THREADS=$j
for ((i=100;i<=2000;i=i+100));do
./gauss-$k $i $j
done;
mv time-par.dat ./$k/time-par-gauss-$j.dat
done;
done;
echo "end openMP"
