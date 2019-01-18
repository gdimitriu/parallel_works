#!/bin/bash
for j in 2 3 4 5 6 7; do
case $j in
	( 2 )
		/etc/init.d/openmosix status;
		rsh c3 /etc/init.d/openmosix status;;
	( 3 ) rsh c4 /etc/init.d/openmosix status;;
	( 4 ) rsh c5 /etc/init.d/openmosix status;;
	( 5 ) rsh c6 /etc/init.d/openmosix status;;
	( 6 ) rsh c8 /etc/init.d/openmosix status;;
esac;
done;
