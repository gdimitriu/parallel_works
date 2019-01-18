#!/bin/bash
#parallel area
for j in 2 3 4 5; do
for k in gj-inv-mpich1 ; do
for ((i=50;i<=1450;i=i+50));do
mpirun -np $j ./$k $i 0.0000001
done;
mv time-par.dat $k-$j.dat
done;
done;
