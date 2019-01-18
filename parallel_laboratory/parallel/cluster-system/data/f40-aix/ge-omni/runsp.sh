#!/bin/sh
for i in 2 4 8 16 24 32 64 128; do
./speedup ../../../serial/aix-new/time-ser-gauss.dat time-par-gauss-$i.dat speedup-gauss-$i.dat
done;
#for i in 2 4 8 16 24 32 64 128; do
#./speedup time-par-gauss-1.dat time-par-gauss-$i.dat speedup1-gauss-$i.dat
#done;