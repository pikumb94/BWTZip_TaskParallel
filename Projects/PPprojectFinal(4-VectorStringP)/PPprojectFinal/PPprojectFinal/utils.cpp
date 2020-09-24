#include "pch.h"
#include "utils.h"

int numCycles(int nBlocks) {
	int remainder = nBlocks % NUM_BLOCKS_PER_STAGE, cicli;

	if (remainder != 0)
		cicli = nBlocks / NUM_BLOCKS_PER_STAGE + 1;
	else
		cicli = nBlocks / NUM_BLOCKS_PER_STAGE;

	return cicli;
}

int numBlocksPerThread(int numThreadStage) {
	return NUM_BLOCKS_PER_STAGE / numThreadStage;
}