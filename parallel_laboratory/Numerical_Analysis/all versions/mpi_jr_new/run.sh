#!/bin/bash
#parallel area
for k in jacobi-row; do
for j in 2 3 4 5 6 7; do
for ((i=50;i<=2500;i=i+50));do
mpirun -np $j ./$k $i 0.0000001
done;
mv time-par.dat $k-mpich126-$j.dat
done;
done;
echo "end mpich"
