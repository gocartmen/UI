#include "generatecppclass.h"

string GenerateCppClass::toUpper(string text)
{
    stringstream upper;
    for(int i=0; i<text.size(); i++){
        upper << (char)toupper(text[i]);
    }

    return upper.str();
}

string GenerateCppClass::toLower(string text)
{
    stringstream lower;
    for(int i=0; i<text.size(); i++){
        lower << (char)tolower(text[i]);
    }

    return lower.str();
}

void GenerateCppClass::saveFile(string fileName, string fileContent)
{
    cout << "saving file (fileName: " << fileName << ") ...";
    fileWriter.open(fileName, ios::out);
    fileWriter << fileContent;
    fileWriter.close();
    cout << "  loaded!!" << endl;
}

void GenerateCppClass::addButtons(string fileName, string className)
{
    for(int i=0; i<blocks.size(); i++){
        if(blocks[i].ID == 1){
            buttons.push_back(i);
        }
    }

    for(int ci=0; ci<buttons.size(); ci++) //iterate multiple times to be sure it is in order
    for(int i=0; i<buttons.size(); i++){
        GLfloat ULx = blocks[buttons[i]].ULx;
        GLfloat ULy = blocks[buttons[i]].ULy;

        for(int j=i+1; j<buttons.size(); j++){
            GLfloat UL2x = blocks[buttons[j]].ULx;
            GLfloat UL2y = blocks[buttons[j]].ULy;
            if(((GLint)(ULx*100) < (GLint)(UL2x*100) && (GLint)(ULy*100) == (GLint)(UL2y*100)) || (GLint)(ULy*100) < (GLint)(UL2y*100)){
                GLuint swap = buttons[i];
                buttons[i] = buttons[j];
                buttons[j] = swap;
            }
        }
    }

    publicString << "\t" << className << "();\n"
                 << "\t~" << className << "();\n"
                 << "\tvoid setMouseInfo(bool isClicked, GLfloat mx, GLfloat my);\n"
                 << "\n";

    functionsString << className << "::" << className << "()\n"
                    << "{\n"
                    << "\tui = new UI();\n"
                    << "\tui->init(\"UI/" << fileName << ".bin\");\n"
                    << "\tui->updateVBOs();\n"
                    << "\tfor(int i=0; i<" << blocks.size() << "; i++){\n"

                    << "\t\tif(ui->getBlockList()[i].ID == 1){\n"
                    << "\t\t\tbuttonVec.push_back(\n"
                    << "\t\t\t\tIButton(\n"
                    << "\t\t\t\t\tui->getBlockList()[i].ULx,\n"
                    << "\t\t\t\t\tui->getBlockList()[i].BRx,\n"
                    << "\t\t\t\t\tui->getBlockList()[i].ULy,\n"
                    << "\t\t\t\t\tui->getBlockList()[i].BRy\n"
                    << "\t\t\t\t)\n"
                    << "\t\t\t);\n"
                    << "\t\t}\n"

                    << "\t\tif(ui->getBlockList()[i].ID == 6){\n"
                    << "\t\t\ticonVec.push_back(\n"
                    << "\t\t\t\tIIcon(\n"
                    << "\t\t\t\t\tui->getBlockList()[i].ULx,\n"
                    << "\t\t\t\t\tui->getBlockList()[i].BRx,\n"
                    << "\t\t\t\t\tui->getBlockList()[i].ULy,\n"
                    << "\t\t\t\t\tui->getBlockList()[i].BRy\n"
                    << "\t\t\t\t)\n"
                    << "\t\t\t);\n"
                    << "\t\t}\n"

                    << "\t}\n"
                    << "}\n"
                    << "\n"
                    << className << "::~" << className << "()\n"
                    << "{\n"
                    << "\tdelete ui;\n"
                    << "}\n"
                    << "\n"
                    << "void " << className << "::setMouseInfo(bool isClicked, GLfloat mx, GLfloat my)\n"
                    << "{\n"
                    << "\tthis->isClicked = isClicked;\n"
                    << "\tthis->mx = mx;\n"
                    << "\tthis->my = my;\n"
                    << "}\n"
                    << "\n";

    privateString << "\tUI * ui = nullptr;\n"
                  << "\tstd::vector<IButton> buttonVec;\n"
                  << "\tstd::vector<IIcon> iconVec;\n"
                  << "\tbool isClicked = false;\n"
                  << "\tGLfloat mx = 0.0f;\n"
                  << "\tGLfloat my = 0.0f;\n";

    publicString << "\tvoid buttons();\n";
    functionsString << "void " << className << "::buttons()\n"
                    << "{\n"
                    << "\tfor(auto it : buttonVec){\n"
                    << "\t\tif(it.isClick(mx, my, isClicked)){\n"
                    << "\t\t\tit.clickFuncPtr();\n"
                    << "\t\t}\n"
                    << "\t}\n"
                    << "}\n"
                    << "\n";
}

void GenerateCppClass::addIcons(string fileName, string className)
{
    for(int i=0; i<blocks.size(); i++){
        if(blocks[i].ID == 6){
            icons.push_back(i);
        }
    }

    for(int ci=0; ci<icons.size(); ci++) //iterate multiple times to be sure it is in order
    for(int i=0; i<icons.size(); i++){
        GLfloat ULx = blocks[icons[i]].ULx;
        GLfloat ULy = blocks[icons[i]].ULy;

        for(int j=i+1; j<icons.size(); j++){
            GLfloat UL2x = blocks[icons[j]].ULx;
            GLfloat UL2y = blocks[icons[j]].ULy;
            if(((GLint)(ULx*100) < (GLint)(UL2x*100) && (GLint)(ULy*100) == (GLint)(UL2y*100)) || (GLint)(ULy*100) < (GLint)(UL2y*100)){
                GLuint swap = icons[i];
                icons[i] = icons[j];
                icons[j] = swap;
            }
        }
    }

    publicString << "\tvoid icons();\n";
    functionsString << "void " << className << "::icons()\n"
                    << "{\n"
                    << "\tfor(auto it : iconVec){\n"
                    << "\t\tif(it.isClick(mx, my, isClicked)){\n"
                    << "\t\t\tit.clickFuncPtr();\n"
                    << "\t\t}\n"
                    << "\t}\n"
                    << "}\n"
                    << "\n";
}

void GenerateCppClass::generateHeaderFile(string fileName, string className)
{
    addButtons(fileName, className);
    addIcons(fileName, className);
    header << "#ifndef " << toUpper(className) << "_H\n"
           << "#define " << toUpper(className) << "_H\n"
           << "\n"
           << "#include <Classes/UI/ui.h>\n"
           << "#include <Classes/UI/Elements/ibutton.h>\n"
           << "#include <Classes/UI/Elements/iicon.h>\n"
           << "\n"
           << "using namespace std;\n"
           << "\n"
           << "class " << className << "\n"
           << "{\n"
           << "public:\n"
           << publicString.str()
           << "private:\n"
           << privateString.str()
           << "};\n"
           << "#endif // " << toUpper(className) << "_H\n";

    saveFile(fileName + ".h", header.str());
}

void GenerateCppClass::generateCppFile(string fileName, string className)
{
    cpp << "#include \"Classes/UI/" << toLower(className) << ".h\"\n"
        << "\n"
        << functionsString.str()
        << "\n";

    saveFile(fileName + ".cpp", cpp.str());
}

GenerateCppClass::GenerateCppClass(vector<block> blocks, string fileName, string className)
{
    for(int i=0; i<blocks.size(); i++){
        this->blocks.push_back(blocks[i]);
    }

    generateHeaderFile(fileName, className);
    generateCppFile(fileName, className);
}
