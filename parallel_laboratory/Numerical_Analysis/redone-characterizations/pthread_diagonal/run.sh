#!/bin/bash
for j in 2; do
for ((i=50;i<=2500;i=i+50));do
./diag-pthread $i $j
done;
mv time-par.dat time-par-diag-pthread-gcc-$j-2500.dat
done;
echo "end pthread"
