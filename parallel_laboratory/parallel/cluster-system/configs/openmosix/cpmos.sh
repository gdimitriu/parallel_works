#!/bin/bash
for i in 2 3 4 5 6 8; do
rcp openmosix.map root@c$i:/etc/openmosix.map
rcp openmosix.config root@c$i:/etc/openmosix/openmosix.config
done;