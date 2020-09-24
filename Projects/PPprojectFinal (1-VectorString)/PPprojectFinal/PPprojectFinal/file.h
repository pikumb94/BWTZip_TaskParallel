#pragma once

#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector <string> readFile(string fileIn);

void writeFile(string fileOut, vector <string> blocks);