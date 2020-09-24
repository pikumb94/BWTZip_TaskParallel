#pragma once

#include <vector>
#include <fstream>
#include <string>

using namespace std;

void readFile(string &fileIn, vector<string*> &blocks);

void writeFile(string &fileOut, vector<string*> &blocks);