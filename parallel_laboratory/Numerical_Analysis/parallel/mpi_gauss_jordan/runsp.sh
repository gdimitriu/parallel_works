#!/bin/sh
for i in 2 4 8 16 24 32 64 128; do
./speedup ~/gabriel/GE/serial/$1/time-ser-$2.dat time-par-$2-$i.dat speedup-$2-$i.dat
done;