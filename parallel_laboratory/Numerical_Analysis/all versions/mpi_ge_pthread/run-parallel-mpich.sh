#!/bin/bash
#parallel area
for j in 2 3; do
for ((i=50;i<=1500;i=i+50));do
mpirun -np $j ./proiect-mpich $i
done;
mv time-par.dat GE-mpich-$j.dat
done;
echo "end GAUSS"
