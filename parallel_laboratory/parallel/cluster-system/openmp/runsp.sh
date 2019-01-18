#!/bin/sh
for i in 2 4; do
./speedup time-ser-$1.dat time-par-$1-$2-$i.dat speedup-$1-$2-$i.dat
done;