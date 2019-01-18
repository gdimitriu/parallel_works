#!/bin/bash
#parallel area
mpdboot --totalnum=5 --rsh=rsh --file=/mnt/work/configs/mpd.hosts
for j in 2 3 4 5; do
#mpdboot --totalnum=$j --rsh=rsh --file=/mnt/work/configs/mpd.hosts
for k in jr-mpich2 ; do
for ((i=50;i<=1500;i=i+50));do
mpirun -np $j ./$k $i 0.0000001
done;
mv time-par.dat $k-$j.dat
done;
#mpdallexit
done;
mpdallexit
for ((i=50;i<=1500;i=i+50));do
./jr $i 0.0000001
done;
mv time-ser.dat jr-ser.dat
