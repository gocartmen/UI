#include "nodeeditor.h"

NodeEditor::NodeEditor()
{

}

void NodeEditor::init()
{
    vbo.reserve(18*maxBlockNum);
    cbo.reserve(24*maxBlockNum);
    tbo.reserve(12*maxBlockNum);

    for(int j=0;j<maxBlockNum;j++){
        vbo.push_back(0);
        vbo.push_back(0);
        vbo.push_back(0);

        vbo.push_back(1);
        vbo.push_back(0);
        vbo.push_back(0);

        vbo.push_back(0);
        vbo.push_back(1);
        vbo.push_back(1);


        vbo.push_back(1);
        vbo.push_back(0);
        vbo.push_back(0);

        vbo.push_back(1);
        vbo.push_back(1);
        vbo.push_back(1);

        vbo.push_back(0);
        vbo.push_back(1);
        vbo.push_back(1);

        for(int i=0;i<6;i++){
            cbo.push_back(255);
            cbo.push_back(255);
            cbo.push_back(255);
            cbo.push_back(255);
        }

        tbo.push_back(0);
        tbo.push_back(1);

        tbo.push_back(1);
        tbo.push_back(1);

        tbo.push_back(0);
        tbo.push_back(0);


        tbo.push_back(1);
        tbo.push_back(1);

        tbo.push_back(1);
        tbo.push_back(0);

        tbo.push_back(0);
        tbo.push_back(0);
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(GLfloat), (GLfloat*) vbo.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, cbo.size() * sizeof(GLubyte), (GLubyte*) cbo.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, tbo.size() * sizeof(GLfloat), (GLfloat*) tbo.data(), GL_STREAM_DRAW);
}

void NodeEditor::updateZoom(GLfloat zoom)
{
    this->zoom = zoom;
    if(zoom < minZoom){
        this->zoom = minZoom;
    }
    if(zoom > maxZoom){
        this->zoom = maxZoom;
    }
}

void NodeEditor::updateData(vector<UI> uis)
{
    vbo.erase(vbo.begin(), vbo.end());
    cbo.erase(cbo.begin(), cbo.end());
    for(int i=0;i<uis.size();i++){
        GLuint numBlock = uis[i].getBlockNum();
        auto blockList = uis[i].getBlockList();

        //iterate through all blocks and sort them by block type (eg: button, background)
        for(int k=0;k<IDCOUNT;k++)
        for(int j=0;j<numBlock;j++){
            if(blockList[j].ID == k){//need to calculate how it takes place on the screen to be viewable
                /*vbo.push_back();
                vbo.push_back();
                vbo.push_back(1.0);

                vbo.push_back();
                vbo.push_back();
                vbo.push_back(1.0);

                vbo.push_back();
                vbo.push_back();
                vbo.push_back(1.0);


                vbo.push_back();
                vbo.push_back();
                vbo.push_back(1.0);

                vbo.push_back();
                vbo.push_back();
                vbo.push_back(1.0);

                vbo.push_back();
                vbo.push_back();
                vbo.push_back(1.0);*/
            }
        }
    }
}

void NodeEditor::updateVBOs()
{

}

void NodeEditor::draw(GLuint program)
{
    glUseProgram(program);

    //updateVBOs();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(glGetAttribLocation(program,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program,"a_vertex"));

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(glGetAttribLocation(program,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program,"a_color"));

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(glGetAttribLocation(program,"a_tex0"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program,"a_tex0"));


    //glBindTexture(GL_TEXTURE_2D, texture);//texturing is under development
    //glDrawArrays(GL_TRIANGLES, 0, vbo.size()/3);


    /*for(int i=0;i<blockNum;i++){
        //glUniform1i(glGetUniformLocation(program.program,"isTextured"), 0);
        glUniform1i(glGetUniformLocation(program.program,"isTextured"), 1);
        glBindTexture(GL_TEXTURE_2D, textures[texture[i]]);//texturing is under development
        glDrawArrays(GL_TRIANGLES, i*6, 6);
    }*/

    glDisableVertexAttribArray(glGetAttribLocation(program,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(program,"a_color"));
    glDisableVertexAttribArray(glGetAttribLocation(program,"a_tex0"));
}
