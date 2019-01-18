#!/bin/bash
#2 4 8 16 24 32 64 128
for k in fast; do
mkdir $k
for j in 1 2 4 8 16 24 32 64 128; do
for ((i=100;i<=2000;i=i+100));do
./mul-$k $i $j
done;
mv time-par.dat ./$k/time-par-mul-$j.dat
mv data.dat ./$k/data-par-mul-$j.dat
done;
done;
echo "end pthread"
