#!/bin/bash
#parallel area
for j in 2 3 4 5; do
for ((i=50;i<=700;i=i+50));do
hpcrun mprun -np $j ./gji-mpi $i
done;
for ((i=800;i<=1100;i=i+100));do
hpcrun mprun -np $j ./gji-mpi-1 $i
hpcrun mprun -np $j ./gji-mpi-1 $i
hpcrun mprun -np $j ./gji-mpi-1 $i
hpcrun mprun -np $j ./gji-mpi-1 $i
hpcrun mprun -np $j ./gji-mpi-1 $i
done;
mv time-par.dat gji-mpi-$j.dat
done;
echo "end mpi"
