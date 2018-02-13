#!/bin/bash
for ((i=50;i<=1300;i=i+50));do
hpcrun ge $i 
done;
mv time-ser.dat ge-ser.dat
