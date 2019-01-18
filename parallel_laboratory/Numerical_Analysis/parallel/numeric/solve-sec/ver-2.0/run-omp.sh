#!/bin/bash
export OMP_NUM_THREADS=2
for ((i=10;i<100;i=i+10));do
./bench-omp 2 t$i.dat.gz
done;
for ((i=100;i<1000;i=i+100));do
./bench-omp 2 t$i.dat.gz
done;
./bench-omp 2 t1000.dat.gz
./bench-omp 2 t3000.dat.gz