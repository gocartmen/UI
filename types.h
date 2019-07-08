#ifndef TYPES_H
#define TYPES_H

//#include <GL/glus.h>
#include <GL/glew.h>

struct block{//for background operations
    GLuint ID = 0;//type of block
    GLfloat ULx = 0.0;
    GLfloat ULy = 0.0;
    GLfloat BRx = 0.0;
    GLfloat BRy = 0.0;
    GLfloat z = 0.0;

    GLubyte r = 0.0;
    GLubyte g = 0.0;
    GLubyte b = 0.0;

    GLuint texID = 0;

    bool isHover = false;
    bool isClicked = false;
};

struct dimension{
    GLfloat x;
    GLfloat y;
};

#endif // TYPES_H
