#!/bin/bash
#parallel area
for j in 2 3; do
for ((i=50;i<=1500;i=i+50));do
mpirun -np $j,scored=c5.windowsnt.tech.pub.ro ./proiect $i
done;
mv time-par.dat GE-mpi-$j.dat
done;
echo "end GAUSS"
