#!/bin/bash
for ((i=100;i<=2000;i=i+100));do
./gauss-serial $i 1
done;
mv time-ser.dat time-ser-gauss.dat
echo "end serial"
