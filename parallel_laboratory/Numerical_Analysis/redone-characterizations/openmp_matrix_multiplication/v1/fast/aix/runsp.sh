#!/bin/sh
for i in 1 2 4 8 16 24 32 64 128; do
./speedup ../../serial/time-ser-mul.dat time-par-mul-$i.dat speedup-mul-$i.dat
done;
for i in 2 4 8 16 24 32 64 128; do
./speedup time-par-mul-1.dat time-par-mul-$i.dat speedup1-mul-$i.dat
done;
