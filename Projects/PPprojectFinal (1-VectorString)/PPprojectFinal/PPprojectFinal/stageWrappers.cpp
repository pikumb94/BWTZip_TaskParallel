#include "pch.h"
#include "utils.h"
#include <omp.h>
#include <stdio.h>
#include "stageWrappers.h"
//#include "stages.h"
#include "ARI.hh"
#include "BWT.hh"
#include "MTF.hh"
#include "ZLE.hh"

using namespace std;

extern vector<string> input, stage1, stage2, output;

void BWTwrapper(int i, int cycle) {
	if (cycle % 2 == 0)
		stage1[i % NUM_BLOCKS_PER_STAGE] = BWT(input[i]);
	else
		stage1[(i % NUM_BLOCKS_PER_STAGE) + NUM_BLOCKS_PER_STAGE] = BWT(input[i]);

	printf("BWT_eseguito: CC %d, block %d, thread %d of %d\n", cycle, i, omp_get_thread_num(), omp_get_num_threads());
}

void MTF_RLEwrapper(int i, int cycle) {
	if (cycle % 2 == 0) {
		stage1[i] = MTF(stage1[i]);
		stage2[i] = ZLE(stage1[i]);
	}
	else {
		stage1[NUM_BLOCKS_PER_STAGE + i] = MTF(stage1[NUM_BLOCKS_PER_STAGE + i]);
		stage2[NUM_BLOCKS_PER_STAGE + i] = ZLE(stage1[NUM_BLOCKS_PER_STAGE + i]);
		}

	printf("RLE_eseguito: CC %d, block %d, thread %d of %d\n", cycle + 1, i, omp_get_thread_num(), omp_get_num_threads());
}

void ARIwrapper(int i, int cycle) {
	if (cycle % 2 == 0)
		output[i + cycle * NUM_BLOCKS_PER_STAGE] = ARI(stage2[i]);
	else
		output[i + cycle * NUM_BLOCKS_PER_STAGE] = ARI(stage2[i + NUM_BLOCKS_PER_STAGE]);

	printf("MTF_eseguito: CC %d, block %d, thread %d of %d\n", cycle + 2, i + cycle * NUM_BLOCKS_PER_STAGE, omp_get_thread_num(), omp_get_num_threads());

}