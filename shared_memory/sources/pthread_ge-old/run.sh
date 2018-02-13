#!/bin/bash
#2 4 8 16 24 32
#for k in fast; do
#mkdir $k
for j in 2 4 8; do
for ((i=50;i<=2500;i=i+50));do
./gauss-fast $i $j
done;
mv time-par.dat time-par-gauss-pthread-gcc-$j-2500.dat
done;
#done;
echo "end pthread"
