#!/bin/bash
#2 4 8 16 24 32 64 128
for k in fast; do
#mkdir $k
for j in 1; do
for ((i=100;i<=2000;i=i+100));do
./gauss-$k $i $j
done;
mv time-par.dat ./$k/time-par-gauss-$j.dat
done;
done;
echo "end pthread"
