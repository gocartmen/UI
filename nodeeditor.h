#ifndef NODEEDITOR_H
#define NODEEDITOR_H

//#include <GL/glus.h>
#include <GL/glew.h>
#include <vector>
#include "ui.h"
#include "types.h"

using namespace std;

/*
NODE EDITOR FUNCTIONALITY
1.: -Display all blocks
    -Info about block type (ID from blocklist data to know the type of block and
                            block index + UI index gives identity to any block in the database)
    -Scrolling zooms in and out
2.: -Creating new Nodes (right click menu (add node, create connection))
    -New node types (eg: animator (plays animated texture))
    -Creating connections (direction of connection is changeable) (opening a menu or closing a menu)
    -New connection types (eg: trigger (onClick, onHover))
3.: -Generating C++ classes
    -Creating main storage class
    -Creating UI classes
    -Creating functions in classes from connection and node data
*/

class NodeEditor
{
private:
    #define IDCOUNT 7
    GLuint maxBlockNum = 500;

    GLuint VBO;
    GLuint CBO;
    GLuint TBO;

    vector<GLfloat> vbo;
    vector<GLubyte> cbo;
    vector<GLfloat> tbo;

    GLfloat zoom = 1.0;
    GLfloat minZoom = 0.1;
    GLfloat maxZoom = 1.0;
public:
    NodeEditor();

    void init();
    void updateZoom(GLfloat zoom);

    void updateData(vector<UI> uis);
    void updateVBOs();

    void draw(GLuint program);
};

#endif // NODEEDITOR_H
