#include "ui.h"

UI::UI()
{

}

void UI::init(string filename)
{
    ifstream myFile;
    myFile.open(filename.c_str(), ios::in | ios::binary);

    myFile.read((char*)(&maxBlockNum), sizeof(maxBlockNum));

    myFile.close();

    cout << "Reserving " << maxBlockNum << " blocks for VBOs." << endl;

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

    load(filename);
}

void UI::update(GLint blockID, bool active)
{
    GLubyte * data = new GLubyte[4*6];

    GLfloat lr = 255 - cbo[blockID*4*6 + 0];
    GLfloat lg = 255 - cbo[blockID*4*6 + 1];
    GLfloat lb = 255 - cbo[blockID*4*6 + 2];

    if(active){
        for(int i=0;i<6;i++){
            data[i*4 + 0] = cbo[blockID*4*6 + i*4 + 0] + (GLubyte)(lr/4);
            data[i*4 + 1] = cbo[blockID*4*6 + i*4 + 1] + (GLubyte)(lg/4);
            data[i*4 + 2] = cbo[blockID*4*6 + i*4 + 2] + (GLubyte)(lb/4);
            data[i*4 + 3] = cbo[blockID*4*6 + i*4 + 3];
        }
    }else{
        for(int i=0;i<6;i++){
            data[i*4 + 0] = cbo[blockID*4*6 + i*4 + 0];
            data[i*4 + 1] = cbo[blockID*4*6 + i*4 + 1];
            data[i*4 + 2] = cbo[blockID*4*6 + i*4 + 2];
            data[i*4 + 3] = cbo[blockID*4*6 + i*4 + 3];
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferSubData(GL_ARRAY_BUFFER, blockID*4*6, sizeof(GLubyte)*4*6, (GLubyte*)data);

    delete [] data;
}

void UI::setTextures(GLuint textures[])
{
    for(int i=0;i<28*3;i++){
        this->textures[i] = textures[i];
    }
}

void UI::setTexture(GLuint ID, GLuint tex)
{
    blockList[ID].texID = tex;

    for(int i=0;i<blockNum;i++){
        texture[i] = blockList[i].texID;
    }
}

GLuint UI::getBlockNum() const
{
    return blockNum;
}

vector<block> UI::getBlockList() const
{
    return blockList;
}

void UI::load(string filename)
{
    actualBlockNum = 0;
    blockList.erase(blockList.begin(), blockList.end());

    for(int i=0;i<maxBlockNum*18;i++){
        vbo[i] = 0;
    }
    for(int i=0;i<maxBlockNum*24;i++){
        cbo[i] = 0;
    }

    ifstream myFile;
    myFile.open(filename.c_str(), ios::in | ios::binary);

    myFile.read((char*)(&blockNum), sizeof(blockNum));

    for(int i=0;i<blockNum;i++){
        struct block newBlock;
        myFile.read((char*)(&(newBlock.ID)),sizeof(newBlock.ID));
        myFile.read(reinterpret_cast<char*>(&(newBlock.ULx)),sizeof(newBlock.ULx));
        myFile.read(reinterpret_cast<char*>(&(newBlock.ULy)),sizeof(newBlock.ULy));
        myFile.read(reinterpret_cast<char*>(&(newBlock.BRx)),sizeof(newBlock.BRx));
        myFile.read(reinterpret_cast<char*>(&(newBlock.BRy)),sizeof(newBlock.BRy));
        myFile.read(reinterpret_cast<char*>(&(newBlock.z)),sizeof(newBlock.z));
        myFile.read((char*)(&(newBlock.r)),sizeof(newBlock.r));
        myFile.read((char*)(&(newBlock.g)),sizeof(newBlock.g));
        myFile.read((char*)(&(newBlock.b)),sizeof(newBlock.b));
        myFile.read((char*)(&(newBlock.texID)),sizeof(newBlock.texID));
        myFile.read((char*)(&(newBlock.isHover)),sizeof(newBlock.isHover));
        myFile.read((char*)(&(newBlock.isClicked)),sizeof(newBlock.isClicked));

        texture.push_back(newBlock.texID);

        blockList.push_back(newBlock);

        vbo[i*18 + 0] = (newBlock.ULx);
        vbo[i*18 + 1] = (newBlock.ULy);
        vbo[i*18 + 2] = (newBlock.z);

        vbo[i*18 + 3] = (newBlock.BRx);
        vbo[i*18 + 4] = (newBlock.ULy);
        vbo[i*18 + 5] = (newBlock.z);

        vbo[i*18 + 6] = (newBlock.ULx);
        vbo[i*18 + 7] = (newBlock.BRy);
        vbo[i*18 + 8] = (newBlock.z);


        vbo[i*18 + 9] = (newBlock.BRx);
        vbo[i*18 + 10] = (newBlock.ULy);
        vbo[i*18 + 11] = (newBlock.z);

        vbo[i*18 + 12] = (newBlock.BRx);
        vbo[i*18 + 13] = (newBlock.BRy);
        vbo[i*18 + 14] = (newBlock.z);

        vbo[i*18 + 15] = (newBlock.ULx);
        vbo[i*18 + 16] = (newBlock.BRy);
        vbo[i*18 + 17] = (newBlock.z);


        for(int j=0;j<6;j++){
            cbo[i*24 + j*4 + 0] = (newBlock.r);
            cbo[i*24 + j*4 + 1] = (newBlock.g);
            cbo[i*24 + j*4 + 2] = (newBlock.b);
            cbo[i*24 + j*4 + 3] = (255);
        }
    }

    myFile.close();
}

void UI::updateVBOs(){
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vbo.size() * sizeof(GLfloat), (GLfloat*) vbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cbo.size() * sizeof(GLubyte), (GLubyte*) cbo.data());
}

void UI::updateTBO(dimension dimensions[], GLuint IDfrom, GLuint IDto, GLint texID)
{
    if(texID == -1){
        for(int i=IDfrom;i<IDto;i++){
            tbo[i*12 + 0] = ((0 - 0.5)*dimensions[i-IDfrom].x + 0.5);
            tbo[i*12 + 1] = ((1 - 0.5)*dimensions[i-IDfrom].y + 0.5);

            tbo[i*12 + 2] = ((1 - 0.5)*dimensions[i-IDfrom].x + 0.5);
            tbo[i*12 + 3] = ((1 - 0.5)*dimensions[i-IDfrom].y + 0.5);

            tbo[i*12 + 4] = ((0 - 0.5)*dimensions[i-IDfrom].x + 0.5);
            tbo[i*12 + 5] = ((0 - 0.5)*dimensions[i-IDfrom].y + 0.5);


            tbo[i*12 + 6] = ((1 - 0.5)*dimensions[i-IDfrom].x + 0.5);
            tbo[i*12 + 7] = ((1 - 0.5)*dimensions[i-IDfrom].y + 0.5);

            tbo[i*12 + 8] = ((1 - 0.5)*dimensions[i-IDfrom].x + 0.5);
            tbo[i*12 + 9] = ((0 - 0.5)*dimensions[i-IDfrom].y + 0.5);

            tbo[i*12 + 10] = ((0 - 0.5)*dimensions[i-IDfrom].x + 0.5);
            tbo[i*12 + 11] = ((0 - 0.5)*dimensions[i-IDfrom].y + 0.5);
        }

        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, tbo.size() * sizeof(GLfloat), (GLfloat*) tbo.data());
    }else{
        for(int i=IDfrom;i<IDto;i++){
            tbo[i*12 + 0] = ((0 - 0.5)*dimensions[texID].x + 0.5);
            tbo[i*12 + 1] = ((1 - 0.5)*dimensions[texID].y + 0.5);

            tbo[i*12 + 2] = ((1 - 0.5)*dimensions[texID].x + 0.5);
            tbo[i*12 + 3] = ((1 - 0.5)*dimensions[texID].y + 0.5);

            tbo[i*12 + 4] = ((0 - 0.5)*dimensions[texID].x + 0.5);
            tbo[i*12 + 5] = ((0 - 0.5)*dimensions[texID].y + 0.5);


            tbo[i*12 + 6] = ((1 - 0.5)*dimensions[texID].x + 0.5);
            tbo[i*12 + 7] = ((1 - 0.5)*dimensions[texID].y + 0.5);

            tbo[i*12 + 8] = ((1 - 0.5)*dimensions[texID].x + 0.5);
            tbo[i*12 + 9] = ((0 - 0.5)*dimensions[texID].y + 0.5);

            tbo[i*12 + 10] = ((0 - 0.5)*dimensions[texID].x + 0.5);
            tbo[i*12 + 11] = ((0 - 0.5)*dimensions[texID].y + 0.5);
        }

        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, tbo.size() * sizeof(GLfloat), (GLfloat*) tbo.data());
    }
}

void UI::draw(GLuint program)
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


    for(int i=0;i<blockNum;i++){
        //glUniform1i(glGetUniformLocation(program.program,"isTextured"), 0);
        glUniform1i(glGetUniformLocation(program,"isTextured"), 1);
        glBindTexture(GL_TEXTURE_2D, textures[texture[i]]);//texturing is under development
        glDrawArrays(GL_TRIANGLES, i*6, 6);
    }

    glDisableVertexAttribArray(glGetAttribLocation(program,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(program,"a_color"));
    glDisableVertexAttribArray(glGetAttribLocation(program,"a_tex0"));
}

GLint UI::controller(GLfloat x, GLfloat y)
{
    actualBlockNum = 10000;

    for(int i=0;i<blockNum;i++){
        if(blockList[i].ULx > x && blockList[i].BRx < x &&
           blockList[i].ULy > y && blockList[i].BRy < y && (blockList[i].ID == 1 || blockList[i].ID == 2 || blockList[i].ID == 6)){
            actualBlockNum = i;
        }
    }

    if(actualBlockNum != 10000){
        return actualBlockNum;
    }else{
        return -1;
    }
}
