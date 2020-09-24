#pragma warning(disable : 4996)
#include "pch.h"
#include <iostream>
#include "file.h"
#include "utils.h"

void readFile(string &fileIn, vector<vector <unsigned char>> &blocks) {

	
	FILE* in = fopen(fileIn.c_str(), "rb");
	int length;

	if (in != NULL)
	{
		//read from file and put in vector of blocks
		for (;;) {

			vector<unsigned char> * tmp_p = new vector<unsigned char> (BLOCK_SIZE);
			tmp_p->resize(BLOCK_SIZE);

			length = fread(&(*tmp_p)[0], 1, BLOCK_SIZE, in);
			if (length <= 0)
				break;

			tmp_p->resize(length);
			blocks.push_back(*tmp_p);

		}
	}
}


void writeFile(string &fileOut, vector<vector <unsigned char>> &blocks) {

	FILE * out = fopen(fileOut.c_str(), "wb");
	
	if (out != NULL) {
		for(int i = 0; i < blocks.size(); i++){
		//while (blocks.size() > 0) {
			//printf("%s | ", blocks[i]->c_str());
			fwrite((char *) blocks[i].data(), 1, blocks[i].size(), out);
			//blocks.erase(blocks.begin());
		}
	}
}