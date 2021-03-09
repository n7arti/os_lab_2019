#!/bin/bash

file="numbers.txt"
av=0
for i in $(cat $file)
do
   av=$(($av + $i))
done
av=$(($av/$(cat numbers.txt | wc -w)))
echo "Average: $av"
echo "Count: $(cat numbers.txt | wc -w)"