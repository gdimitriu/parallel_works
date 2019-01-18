#!/bin/bash
for j in 2 4 9 16 25; do
	for((i=10;i<100;i=i+10)) ;do
	./bench-pthread0 $j a$i.dat.gz b$i.dat.gz;
	done;
	for((i=100;i<1000;i=i+100));do
	./bench-pthread0 $j a$i.dat.gz b$i.dat.gz;
	done;
	./bench-pthread0 $j a1000.dat.gz b1000.dat.gz;
done;
mv speedup.dat  speedup-col-pthread0.dat
mv time-ser.dat time-ser-col-pthread0.dat
mv time-par.dat time-par-col-pthread0.dat
mv eroare.dat err-col-pthread0.dat

for j in 2 4 9 16 25; do
	for((i=10;i<100;i=i+10)) ;do
	./bench-pthread1 $j a$i.dat.gz b$i.dat.gz;
	done;
	for((i=100;i<1000;i=i+100));do
	./bench-pthread1 $j a$i.dat.gz b$i.dat.gz;
	done;
	./bench-pthread1 $j a1000.dat.gz b1000.dat.gz;
done;
mv speedup.dat  speedup-row-pthread1.dat
mv time-ser.dat time-ser-row-pthread1.dat
mv time-par.dat time-par-row-pthread1.dat
mv eroare.dat err-row-pthread1.dat
