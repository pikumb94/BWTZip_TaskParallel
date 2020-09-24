#pragma once

#include <vector>
#include <fstream>

using namespace std;

void readFile(string &fileIn, vector<vector <unsigned char>*> &blocks);

void writeFile(string &fileOut, vector<vector <unsigned char>*> &blocks);