#!/bin/bash
for i in 2 4 10 20; do
	for ((j=10;j<100;j=j+10)); do
	./bench-sth -nw $1 $i t$j.dat.gz
	done;
	for ((j=100;j<1000;j=j+100));do
	./bench-sth -nw $1 $i t$j.dat.gz
	done;
	./bench-sth -nw $1 $i t1000.dat.gz
	./bench-sth -nw $1 $i t3000.dat.gz
done;