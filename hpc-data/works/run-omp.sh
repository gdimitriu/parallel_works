#!/bin/bash
for j in 2 3 4 5;do
for ((i=50;i<=1300;i=i+50));do
hpcrun jr-omp $i 0.0000001 $j
done;
mv time-par.dat jr-omp-$j.dat
done;
