#!/bin/bash
#serial area
for k in gj-inv-gcc ; do
for ((i=1350;i<=1450;i=i+50));do
./$k $i 0.0000001
done;
mv time-ser.dat $k.dat
done;
