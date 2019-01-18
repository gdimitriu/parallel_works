#!/bin/bash
#for ((i=50;i<=700;i=i+50));do
#hpcrun gji $i 1
#done;
for ((i=800;i<=1300;i=i+100));do
hpcrun gji_1 $i 1
hpcrun gji_1 $i 1
hpcrun gji_1 $i 1
hpcrun gji_1 $i 1
hpcrun gji_1 $i 1
done;
mv time-ser.dat gji-ser.dat
