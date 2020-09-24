# BWTZip_TaskParallel
Parallel Programming Project: Task Parallel Lossless Data Compression  Based on the Burrows-Wheeler Transform.

The Burrows Wheeler transform is a block sorting lossless data compression algorithm, that provides compression ratios within a few percent of the best lossless statistical algorithms while using much less computational resources. We were inspired by the task parallel message passing version of BWTZip called mpibwtzip a three stage pipeline designed to work on cluster machines.
To enable parallelism in our project, we’ve used the OpenMP API In this way we could reuse the C++ implementation of the BWTZip compressor stages.

The following project focuses only on the parallelization of the data compression.
Four versions of the program with different data types are implemented and execution times are reported in "ExperimentsPP.xlsx".
See "PresentazionePP" for further information.
