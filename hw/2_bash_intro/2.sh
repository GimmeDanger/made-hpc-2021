#!/bin/bash

# Task 2
# Initialize the array of 10-20 elements and organize FOR loop
# printing the elements of array

echo "printing array elements:"
arr=("0l" "1l" "2l" "3l" "4l" "5l" "6l" "7l" "8l" "9l")
for i in `seq 1 ${#arr[@]}`; do
    echo ${arr[$i-1]}
done
