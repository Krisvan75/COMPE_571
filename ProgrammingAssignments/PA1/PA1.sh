#!/bin/bash

echo "Building..."
make clean
make


echo "Running workloads..."

N_1=100000000
N_2=1000000000
N_3=10000000000

NT_1=2
NT_2=4
NT_3=8

echo "Starting sequential Workloads..."
echo "----------------------------------------------------------------------------------------------------------------------"
./seqWL $N_1
./seqWL $N_2
./seqWL $N_3
echo "----------------------------------------------------------------------------------------------------------------------"
echo "Sequential Workloads complete."

echo "Starting Multithreaded Workloads..."
echo "----------------------------------------------------------------------------------------------------------------------"
./mthreads $N_1 $NT_1
./mthreads $N_1 $NT_2
./mthreads $N_1 $NT_3
./mthreads $N_2 $NT_1
./mthreads $N_2 $NT_2
./mthreads $N_2 $NT_3
./mthreads $N_3 $NT_1
./mthreads $N_3 $NT_2
./mthreads $N_3 $NT_3
echo "----------------------------------------------------------------------------------------------------------------------"

echo "Multithreaded Workloads complete."

echo "Starting Multitasking Workloads..."
echo "----------------------------------------------------------------------------------------------------------------------"
./mtasks $N_1 $NT_1
./mtasks $N_1 $NT_2
./mtasks $N_1 $NT_3
./mtasks $N_2 $NT_1
./mtasks $N_2 $NT_2
./mtasks $N_2 $NT_3
./mtasks $N_3 $NT_1
./mtasks $N_3 $NT_2
./mtasks $N_3 $NT_3
echo "----------------------------------------------------------------------------------------------------------------------"
echo "Multitasking Workloads complete."


echo "All workloads complete."


