#!/bin/bash
for j in gcc icc;do
for ((i=50;i<=2500;i=i+50));do
./jacobi-$j $i 0.0000001
done;
mv time-ser.dat ./time-ser-jacobi-row-$j-2500.dat
done;
echo "end serial Jacobi row"
