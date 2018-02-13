#!/bin/bash
for ((i=50;i<=1300;i=i+50));do
hpcrun jr $i 0.0000001
done;
mv time-ser.dat jr-ser.dat
