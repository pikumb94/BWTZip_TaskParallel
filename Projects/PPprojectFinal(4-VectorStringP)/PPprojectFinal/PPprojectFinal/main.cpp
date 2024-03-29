// PPprojectFinal.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include "pch.h"
#include <iostream>
#include <omp.h>
#include <chrono>
#include "utils.h"
#include "file.h"
#include "stageWrappers.h"

using namespace std::chrono;

int cycles;
vector <string*> input, stage1/*[2 * NUM_BLOCKS_PER_STAGE]*/, stage2/*[2 * NUM_BLOCKS_PER_STAGE]*/, output;
//vector <string*> inputTest, outputTest;

void firstStage(int* i) {
	int blocksPerThread = numBlocksPerThread(NUM_THREADS_FIRST); //we don't consider the possibility that NUM_BLOCK_PER_STAGE is not multiple of NUM_THREAD_STAGE. Questa cosa non serve quasi a niente ihih
#pragma omp parallel num_threads(NUM_THREADS_FIRST)
	if (*i == (cycles - 1) && (input.size() % NUM_BLOCKS_PER_STAGE) != 0) {
//#pragma omp parallel num_threads(NUM_THREADS_FIRST)
#pragma omp for schedule(dynamic, blocksPerThread)
		for (int j = *i * NUM_BLOCKS_PER_STAGE; j < *i *NUM_BLOCKS_PER_STAGE + (input.size() % NUM_BLOCKS_PER_STAGE); j++)
			BWTwrapper(&j, i);
	}
	else {
//#pragma omp parallel num_threads(NUM_THREADS_FIRST)
#pragma omp for schedule(dynamic, blocksPerThread)
		for (int j = *i * NUM_BLOCKS_PER_STAGE; j < (*i + 1) * NUM_BLOCKS_PER_STAGE; j++) //  i*NUM_BLOCKS_PER_STAG + ( NUM_ELEM % NUM_BLOCK_PER_STAGE) 
			BWTwrapper(&j, i);
	}
}

void secondStage(int* i) {
	int blocksPerThread = numBlocksPerThread(NUM_THREADS_SECOND); //we don't consider the possibility that NUM_BLOCK_PER_STAGE is not multiple of NUM_THREAD_STAGE. Questa cosa non serve quasi a niente ihih

	if (*i == (cycles - 1) && (input.size() % NUM_BLOCKS_PER_STAGE) != 0) {
#pragma omp parallel num_threads(NUM_THREADS_SECOND)
#pragma omp for schedule(dynamic, blocksPerThread)
		for (int j = 0; j < input.size() % NUM_BLOCKS_PER_STAGE; j++)
			MTF_RLEwrapper(&j, i);
	}
	else {
#pragma omp parallel num_threads(NUM_THREADS_SECOND)
#pragma omp for schedule(dynamic, blocksPerThread)
		for (int j = 0; j < NUM_BLOCKS_PER_STAGE; j++)
			MTF_RLEwrapper(&j, i);
	}
}

void thirdStage(int* i) {
	int blocksPerThread = numBlocksPerThread(NUM_THREADS_THIRD); //we don't consider the possibility that NUM_BLOCK_PER_STAGE is not multiple of NUM_THREAD_STAGE. Questa cosa non serve quasi a niente ihih
	if (*i == (cycles - 1) && (input.size() % NUM_BLOCKS_PER_STAGE) != 0) {
#pragma omp parallel num_threads(NUM_THREADS_THIRD)
#pragma omp for schedule(dynamic, blocksPerThread)
		for (int j = 0; j < input.size() % NUM_BLOCKS_PER_STAGE; j++)
			ARIwrapper(&j, i);
	}
	else {
#pragma omp parallel num_threads(NUM_THREADS_THIRD)
#pragma omp for schedule(dynamic, blocksPerThread)
		for (int j = 0; j < NUM_BLOCKS_PER_STAGE; j++)
			ARIwrapper(&j, i);
	}
}

int main(int argc, char* argv[]) {

	/*if (argc > 1) {
		freopen(argv[1], "rb", stdin);
		fprintf(stderr, "%s", argv[1]);
	}
	else
		fprintf(stderr, "stdin");
	fprintf(stderr, " to ");
	if (argc > 2) {
		freopen(argv[2], "wb", stdout);
		fprintf(stderr, "%s", argv[2]);
	}
	else
		fprintf(stderr, "stdout");
	fprintf(stderr, "\n");*/

	//inizializza input
	string fileIn ("obj2.txt");
	readFile(fileIn, input);

	fileIn.~basic_string();

	cycles = numCycles(input.size());

	stage1.resize(2 * NUM_BLOCKS_PER_STAGE);
	stage2.resize(2 * NUM_BLOCKS_PER_STAGE);
	output.resize(input.size());

	int i = 0, j = 0;

	//abilita il parallelismo innestato
	omp_set_nested(1);

	//verifica delle funzioncine appena scritte
	printf("il numero di blocchi letti: %d \n", input.size());
	printf("il numero di cicli da eseguire: %d \n", cycles);

	//verifica della lettura da file
	/*for (int n = 0; n < input.size(); n++) {
		printf("block %d: %s\n", n, input[n]->c_str());
	}*/

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	
#pragma omp parallel num_threads(NUM_STAGES_PIPELINE)
	{
		while (i < cycles + NUM_STAGES_PIPELINE - 1) // cycles represents the number of cycles that the every stage has to do. NB: 
		{
#pragma omp sections
			{
#pragma omp section
				{
					if (i < cycles) {
						firstStage(&i);
					}
				}
#pragma omp section
				{
					int m = i - 1;
					if ( m >= 0 && m < cycles) {
						secondStage(&m);
					}
				}
#pragma omp section
				{
					int n = i - 2;
					if ( n >= 0 && n < cycles) {
						thirdStage(&n);
					}
				}
			}
#pragma omp single
			{
				i++;
				printf("ROUND %d, thread %d\n", i, omp_get_thread_num());
			}
		}
	}
	
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();

	cout << "Durata: ";
	cout << duration <<endl;

	//verifica scrittura su file
	//printa pure i blocchi dell'output
	string fileOut("outTest.txt");
	writeFile(fileOut, output);
}

// Per eseguire il programma: CTRL+F5 oppure Debug > Avvia senza eseguire debug
// Per eseguire il debug del programma: F5 oppure Debug > Avvia debug

// Suggerimenti per iniziare: 
//   1. Usare la finestra Esplora soluzioni per aggiungere/gestire i file
//   2. Usare la finestra Team Explorer per connettersi al controllo del codice sorgente
//   3. Usare la finestra di output per visualizzare l'output di compilazione e altri messaggi
//   4. Usare la finestra Elenco errori per visualizzare gli errori
//   5. Passare a Progetto > Aggiungi nuovo elemento per creare nuovi file di codice oppure a Progetto > Aggiungi elemento esistente per aggiungere file di codice esistenti al progetto
//   6. Per aprire di nuovo questo progetto in futuro, passare a File > Apri > Progetto e selezionare il file con estensione sln
