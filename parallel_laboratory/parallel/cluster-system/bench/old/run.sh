#!/bin/bash
#parallel area
for k in jacobi-row-new gs; do
for j in 2 3 4; do
for ((i=50;i<=2500;i=i+50));do
mpirun -np $j ./$k $i 0.0000001
done;
mv time-par.dat $k-mpich126-$j.dat
done;
done;
echo "end mpich"

for j in 2 3 4 5 6 7; do
case $j in
	( 2 )
		/etc/init.d/openmosix start;
 		rsh c3 /etc/init.d/openmosix start;;
	( 3 ) rsh c4 /etc/init.d/openmosix start;;
	( 4 ) rsh c5 /etc/init.d/openmosix start;;
	( 5 ) rsh c6 /etc/init.d/openmosix start;;
	( 6 ) rsh c8 /etc/init.d/openmosix start;;
esac;
for k in jacobi-row ge; do
for ((i=50;i<=2500;i=i+50));do
mpirun -np $j ./$k $i 0.0000001
done;
mv time-par.dat $k-mpich126-openmosix-$j.dat
done;
done;
echo "end of characterization"
echo "stoping the system"
for i in c2 c3 c4 c5 c6 c8;do
	rsh $i /etc/init.d/openmosix stop
done;
for i in c3 c4 c5 c6 c8; do
	rsh $i umount /mnt/work
done;
for i in c3 c4 c5 c8 c6 c2;do
	rsh $i /sbin/shutdown -t now
done;

