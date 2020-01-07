#!/bin/bash

## Give the Job a descriptive name
#PBS -N testjob

## Output and error files
#PBS -o testjob.out
#PBS -e testjob.err

## Limit memory, runtime etc.
#PBS -l walltime=00:02:00

## How many nodes:processors_per_node should we get?
# PBS -l nodes=sandman:ppn=64

## Start 
##echo "PBS_NODEFILE = $PBS_NODEFILE"
##cat $PBS_NODEFILE

## Run the job (use full paths to make sure we execute the correct thing) 
## NOTE: Fix the path to show to your executable! 

cd /home/parallel/parlab04/a3/z1
module load openmpi/1.8.3
MT_CONF=0,8,16,24 ./accounts


