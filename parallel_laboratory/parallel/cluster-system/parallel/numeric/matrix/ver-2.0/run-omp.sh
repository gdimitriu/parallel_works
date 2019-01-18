#!/bin/bash
for j in 2 3 4; do
	export OMP_NUM_THREADS=$j
	for((i=10;i<100;i=i+10)) ;do
	./bench-omp_1 $j a$i.dat.gz b$i.dat.gz;
	done;
	for((i=100;i<1000;i=i+100));do
	./bench-omp_1 $j a$i.dat.gz b$i.dat.gz;
	done;
	./bench-omp_1 $j a1000.dat.gz b1000.dat.gz;
done;
mv eroare.dat err_1.dat
mv speedup.dat speedup_1.dat
mv time-ser.dat time-ser_1.dat
mv time-par.dat time-par_1.dat
for j in 2 3 4; do
	export OMP_NUM_THREADS=$j
	for((i=10;i<100;i=i+10)) ;do
	./bench-omp $j a$i.dat.gz b$i.dat.gz;
	done;
	for((i=100;i<1000;i=i+100));do
	./bench-omp $j a$i.dat.gz b$i.dat.gz;
	done;
	./bench-omp $j a1000.dat.gz b1000.dat.gz;
done;
