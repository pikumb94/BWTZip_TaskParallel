#pragma warning(disable : 4996)
#include "pch.h"
#include <iostream>
#include "file.h"
#include "utils.h"

vector <string> readFile(string fileIn) {

	char tmp[BLOCK_SIZE];
	vector <string> blocks;
	FILE* in = fopen(fileIn.c_str(), "rb");
	int length;

	if (in != NULL)
	{
		//read from file and put in vector of blocks
		for (;;) {
			

			length = fread((char*)tmp, 1, BLOCK_SIZE, in);
			if (length <= 0)
				break;
			string myarray(tmp, length);
			blocks.push_back(myarray);
		}
	}

	return blocks;
}


void writeFile(string fileOut, vector <string> blocks) {

	FILE * out = fopen(fileOut.c_str(), "wb");
	
	if (out != NULL) {
		for(int i = 0; i < blocks.size(); i++){
		//while (blocks.size() > 0) {
			printf("%s | ", blocks[i].c_str());
			fwrite((char *)blocks[i].c_str(), 1, blocks[i].size(), out);
			//blocks.erase(blocks.begin());
		}
	}
}