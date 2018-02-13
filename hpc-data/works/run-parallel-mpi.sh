#!/bin/bash
#parallel area
for j in 2 3 4 5; do
for ((i=50;i<=1300;i=i+50));do
hpcrun mprun -np $j ./ge-mpi $i 
done;
mv time-par.dat ge-mpi-$j.dat
done;
echo "end mpi"
