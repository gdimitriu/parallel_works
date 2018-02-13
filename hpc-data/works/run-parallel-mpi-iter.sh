#!/bin/bash
#parallel area
for j in 2 3 4 5; do
for ((i=50;i<=1300;i=i+50));do
hpcrun mprun -np $j ./jr-mpi $i 0.0000001
done;
mv time-par.dat jr-mpi-$j.dat
done;
echo "end mpi"
