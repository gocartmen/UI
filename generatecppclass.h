#ifndef GENERATECPPCLASS_H
#define GENERATECPPCLASS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "types.h"

using namespace std;

class GenerateCppClass
{
private:
    stringstream header;
    stringstream cpp;

    fstream fileWriter;

    vector<block> blocks;

    string toUpper(string text);
    string toLower(string text);

    void saveFile(string fileName, string fileContent);

    void generateHeaderFile(string fileName, string className);
    void generateCppFile(string fileName, string className);
public:
    GenerateCppClass(vector<block> blocks, string fileName, string className);
};

#endif // GENERATECPPCLASS_H
