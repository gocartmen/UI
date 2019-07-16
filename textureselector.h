#ifndef TEXTURESELECTOR_H
#define TEXTURESELECTOR_H

#include "ui.h"
#include "textwriter.h"

using namespace std;

class TextureSelector
{
private:
    UI * ui = NULL;
    UI * ui2 = NULL;
    TextWriter * folderListing = NULL;
    vector<string> folderContent;
    string folderPath;

    bool isSelected = false;
    bool isUpdate = false;
    bool isRemoved = false;
    GLint direction = -1;
    string newPath;
    GLuint folderCount = 0;

    GLuint scrollID = 1;
    GLuint scrollPageNum = 0;
    GLuint actualScrollPage = 0;
    GLuint scrollLength = 14;

    GLuint activeUI = 0;

    GLuint textTexture;

    GLfloat mx = 0;
    GLfloat my = 0;
    bool clicked = false;

    GLuint textures[28*3];
    struct dimension dimensions[28*3];
    GLuint texturesSelect[28*3];
    struct dimension dimensionsSelect[28*3];

    GLuint activeTex = 0;
public:
    TextureSelector();

    void setMouse(GLfloat mx, GLfloat my, bool clicked);
    void initUI(vector<string> &folderContent, GLuint texture);
    void drawUI(GLuint program, GLuint width, GLuint height);

    void updateText(vector<string> &folderContent);

    bool getIsUpdate();
    void setIsUpdate(bool value);
    GLint getDirection();
    void setDirection(GLint value);
    string getNewPath();
    GLuint getFolderCount() const;
    void setFolderCount(const GLuint &value);
    string getFolderPath() const;
    void setFolderPath(const string &value);
    bool getIsSelected() const;
    void setIsSelected(bool value);
    string getSelected();

    void setTextures(GLuint textures[], struct dimension dimensions[]);
    void setTexturesSelect(GLuint texturesSelect[], struct dimension dimensionsSelect[]);
    GLuint getActiveTex() const;
    void setActiveTex(const GLuint &value);
    bool getIsRemoved() const;
    void setIsRemoved(bool value);
};

#endif // TEXTURESELECTOR_H
