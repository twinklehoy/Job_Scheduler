# Process Scheduler Simulation

A C implementation of a Round-Robin process scheduler that simulates CPU scheduling algorithms.

## Overview

This project implements a Round-Robin process scheduler that manages the execution of multiple processes. The scheduler loads processes from an input file, executes them according to the Round-Robin algorithm, and outputs detailed statistics about their execution.

## Features

- **Round-Robin Scheduling**: Processes are executed in a cyclic manner, each getting a fair time slice
- **Process Queue Management**: Efficient implementation of a circular queue for process management
- **Detailed Statistics**: Records and reports key metrics including:
  - Maximum waiting time for each process
  - Average maximum waiting time across all processes
  - Total waiting time for each process
  - Average total waiting time across all processes

## Components

The project consists of three main components:

1. **Processo.c/h**: Defines the `Processo` structure and related functions
2. **QueueProce.c/h**: Implements the circular queue data structure for process management
3. **Scheduler_Tester.c**: Contains the main program logic and testing functionality

## Process Structure

Each process contains the following information:
- `ide`: Process identifier
- `timeLeft`: Remaining execution time
- `end`: Completion timestamp
- `total`: Total waiting time
- `max`: Maximum waiting time

## Usage

### Compilation

You can either:

1. Compile the project using gcc:

```bash
gcc -o scheduler Scheduler_Tester.c QueueProce.c Processo.c -std=c99
```
2. Execute the `compile.bat` file
   
### Execution

Run the program with the following command-line arguments:

```bash
./scheduler <input_file> <output_file> <timeLoading> <timeSlice>
```

Where:
- `<input_file>`: File containing the process data
- `<output_file>`: File where results will be written
- `<timeLoading>`: Time units required to load a process
- `<timeSlice>`: Maximum time units a process can execute before being preempted

### Input File Format

The input file should have the following format:
```
n
t1
t2
...
tn
```

Where:
- `n`: Number of processes
- `t1, t2, ..., tn`: Execution time required for each process

### Output

The program generates detailed output that includes:
- Step-by-step execution of each process (loading, running, extraction)
- Final state of each process (end time, maximum wait time, total wait time)
- Statistical summary (average maximum wait, maximum of maximum waits, average total wait)

## Algorithm

The Round-Robin scheduler follows these steps:
1. Load a process from the queue
2. Execute the process for a maximum of `timeSlice` units (or until completion if it's the only process)
3. If the process hasn't completed, put it back in the queue
4. Repeat until all processes have completed

## Error Handling

The program includes robust error handling for:
- Missing command-line arguments
- File opening failures
- Memory allocation errors
