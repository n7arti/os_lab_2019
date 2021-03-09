#!/bin/bash

echo > numbers.txt 
count=0
while [ $count -ne 150 ]
do
    od -A n -t d -N 1 /dev/urandom >> numbers.txt
    count=$(($count + 1))
done