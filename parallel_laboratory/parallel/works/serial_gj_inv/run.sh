#!/bin/bash
for j in gcc icc; do
for ((i=50;i<=2500;i=i+50));do
./gaussJ-$j $i 1
done;
mv time-ser.dat time-ser-gaussJ-$j-2500.dat
done;
echo "end serial GJ"
