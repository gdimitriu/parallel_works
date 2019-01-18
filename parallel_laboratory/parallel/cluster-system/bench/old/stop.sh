#!/bin/bash
/etc/init.d/openmosix start;
for i in 3 4 5; do
rsh c$i /etc/init.d/openmosix stop;
rsh c$i umount /mnt/work
rsh c$i /sbin/init 0
done;