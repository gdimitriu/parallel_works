#!/bin/bash
for ((i=100;i<=2000;i=i+100));do
./mul-serial $i 1
done;
mv time-ser.dat time-ser-mul.dat
#mv data.dat data-ser-mul.dat
echo "end serial"
