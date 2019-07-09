#ifndef GENERATECPPCLASS_H
#define GENERATECPPCLASS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "types.h"

using namespace std;

class GenerateCppClass
{
private:
    stringstream header;
    stringstream cpp;

    fstream fileWriter;

    vector<block> blocks;
    vector<GLuint> buttons;
    vector<GLuint> icons;
    stringstream publicString;
    stringstream privateString;
    stringstream functionsString;

    string toUpper(string text);
    string toLower(string text);

    void saveFile(string fileName, string fileContent);

    void addButtons(string fileName, string className);
    void addIcons(string fileName, string className);

    void generateHeaderFile(string fileName, string className);
    void generateCppFile(string fileName, string className);
public:
    GenerateCppClass(vector<block> blocks, string fileName, string className);
};

#endif // GENERATECPPCLASS_H
