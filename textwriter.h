#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
//#include <GL/glus.h>
#include <GL/glew.h>

using namespace std;

class TextWriter
{
private:
    struct keys{
        GLfloat x1;
        GLfloat y1;
        GLfloat x2;
        GLfloat y2;
    };
    struct pos{
        GLfloat x;
        GLfloat y;
    };
    map<char,struct keys> translator;
    vector<string> text;
    vector<struct pos> textPos;

    GLuint VBO;
    GLuint CBO;
    GLuint TBO;

    vector<GLfloat> vbo;
    vector<GLubyte> cbo;
    vector<GLfloat> tbo;

    void loadData(ifstream &abcData, keys &data/*, ofstream &abcDataSave, bool isModify = false*/);
public:
    TextWriter();

    void addText(string newText, GLfloat x, GLfloat y);
    void updateText(GLuint szam, string newText, GLubyte r = 0, GLubyte g = 0, GLubyte b = 0);//continue here
    void drawText(GLuint program, GLuint texture);
};

#endif // TEXTWRITER_H
