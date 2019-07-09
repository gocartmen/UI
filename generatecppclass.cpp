#include "generatecppclass.h"

string GenerateCppClass::toUpper(string text)
{
    stringstream upper;
    for(int i=0; i<text.size(); i++){
        upper << toupper(text[i]);
    }

    return upper.str();
}

string GenerateCppClass::toLower(string text)
{
    stringstream lower;
    for(int i=0; i<text.size(); i++){
        lower << tolower(text[i]);
    }

    return lower.str();
}

void GenerateCppClass::saveFile(string fileName, string fileContent)
{

}

void GenerateCppClass::generateHeaderFile(string fileName, string className)
{
    header << "#ifndef " << toUpper(className) << "_H\n"
           << "#define " << toUpper(className) << "_H\n"
           << "\n"
           << "#include <GL/glew.h>;\n"
           << "\n"
           << "using namespace std;\n"
           << "\n"
           << "class " << className << "\n"
           << "{\n"
           << "public:\n"

           << "private:\n"

           << "};\n"
           << "#endif // " << toUpper(className) << "_H\n";

    saveFile(fileName, header.str());
}

void GenerateCppClass::generateCppFile(string fileName, string className)
{
    cpp << "#include \"" << toLower(className) << ".h\"\n"

        << "\n";
}

GenerateCppClass::GenerateCppClass(vector<block> blocks, string fileName, string className)
{
    for(auto actualBlock : blocks){
        this->blocks.push_back(actualBlock);
    }

    generateHeaderFile(fileName, className);
    generateCppFile(fileName, className);
}
