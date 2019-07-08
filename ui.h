#ifndef UI_H
#define UI_H

//#include <GL/glus.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "types.h"


using namespace std;

class UI
{
private:
    vector<struct block> blockList;

    GLuint blockNum = 0;
    GLuint maxBlockNum = 0;
    GLuint actualBlockNum = 0;

    GLuint VBO;
    GLuint CBO;
    GLuint TBO;

    vector<GLfloat> vbo;
    vector<GLubyte> cbo;
    vector<GLfloat> tbo;

    bool isActive = false;

    vector<GLuint> texture;//texture vector will contain the texture ID-s for textures loaded
    GLuint textures[28*3];

    void load(string filename);
public:
    UI();

    void init(string filename);
    void updateVBOs();

    void updateTBO(struct dimension dimensions[], GLuint IDfrom, GLuint IDto, GLint texID = -1);

    void update(GLint blockID, bool active);
    void draw(GLuint program);

    GLint controller(GLfloat x, GLfloat y);

    void setTextures(GLuint textures[]);
    void setTexture(GLuint ID, GLuint tex);
    GLuint getBlockNum() const;
    vector<block> getBlockList() const;
};

#endif // UI_H
