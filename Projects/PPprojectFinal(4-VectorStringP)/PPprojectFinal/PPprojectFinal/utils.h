#pragma once

#include <vector>
#include <string>

#define BLOCK_SIZE 900000

#define NUM_STAGES_PIPELINE 3
#define NUM_BLOCKS_PER_STAGE 4
#define NUM_THREADS_FIRST 4
#define NUM_THREADS_SECOND 1
#define NUM_THREADS_THIRD 1

//this function takes the number of blocks to be processed and return the number of the cycles to be executed by the pipeline
int numCycles(int nBlocks);

//takes in input the number of threads in that stage and return the "average" (truncated to int) number of blocks to be processed by each thread
//NB NEVER set NUM_BLOCKS_PER_STAGE less than the NUM_THREADS in that stage. However we use this function just to "balance" the blocks to thread in the "inner" parallel for loop
int numBlocksPerThread(int numThreadStage);