#!/bin/bash
for j in 2 4 9 16 25; do
	for((i=10;i<100;i=i+10)) ;do
	./bench-proc0 $j a$i.dat.gz b$i.dat.gz;
	done;
	for((i=100;i<1000;i=i+100));do
	./bench-proc0 $j a$i.dat.gz b$i.dat.gz;
	done;
	./bench-proc0 $j a1000.dat.gz b1000.dat.gz;
done;
mv speedup.dat speedup-col-proc0.dat
mv time-ser.dat time-ser-col-proc0.dat
mv time-par.dat time-par-col-proc0.dat
mv eroare.dat err-col-proc0.dat

for j in 2 4 9 16 25; do
	for((i=10;i<100;i=i+10)) ;do
	./bench-proc1 $j a$i.dat.gz b$i.dat.gz;
	done;
	for((i=100;i<1000;i=i+100));do
	./bench-proc1 $j a$i.dat.gz b$i.dat.gz;
	done;
	./bench-proc1 $j a1000.dat.gz b1000.dat.gz;
done;
mv speedup.dat speedup-row-proc1.dat
mv time-ser.dat time-ser-row-proc1.dat
mv time-par.dat time-par-row-proc1.dat
mv eroare.dat err-row-proc1.dat
