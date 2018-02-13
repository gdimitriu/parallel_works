#!/bin/sh
for i in 1 2 4 8 16 24 32 64 128; do
./speedup ../../../serial/aix/time-ser-gaussJ.dat time-par-gaussJ-$i.dat speedup-gaussJ-$i.dat
done;
for i in 2 4 8 16 24 32 64 128; do
./speedup time-par-gaussJ-1.dat time-par-gaussJ-$i.dat speedup1-gaussJ-$i.dat
done;