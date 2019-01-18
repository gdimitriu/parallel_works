#!/bin/bash
#2 4 8 16 24 32
for k in fast fast1; do
mkdir $k
for j in 2 4 8 16 24 32; do
export OMP_NUM_THREADS=$j
for ((i=100;i<=2000;i=i+100));do
./jacobi-$k $i $j
done;
mv speedup.dat ./$k/speedup-jacobi-$j.dat
mv time-par.dat ./$k/time-par-jacobi-$j.dat
mv time-ser.dat ./$k/time-ser-jacobi-$j.dat
done;
done;
