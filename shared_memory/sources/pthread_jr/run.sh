#!/bin/bash
#parallel area
for k in pthread; do
for j in 2; do
for ((i=50;i<=2500;i=i+50));do
./jacobi-$k $i 0.0000001 $j
done;
mv time-par.dat ./time-jacobi-row-$k-$j-2500.dat
done;
done;
echo "end"
