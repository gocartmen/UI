#include "textureselector.h"

GLuint TextureSelector::getFolderCount() const
{
    return folderCount;
}

void TextureSelector::setFolderCount(const GLuint &value)
{
    folderCount = value;
}

string TextureSelector::getFolderPath() const
{
    return folderPath;
}

void TextureSelector::setFolderPath(const string &value)
{
    folderPath = value;
}

bool TextureSelector::getIsSelected() const
{
    return isSelected;
}

void TextureSelector::setIsSelected(bool value)
{
    isSelected = value;
}

string TextureSelector::getSelected()
{
    return folderContent[scrollID-1 + actualScrollPage*scrollLength];
}

void TextureSelector::setTextures(GLuint textures[], dimension dimensions[])
{
    cout << "before for" << endl;
    for(int i=0;i<28*3;i++){
        this->textures[i] = textures[i];
    }
    cout << "after for" << endl;
    ui->setTextures(textures);
    cout << "between UIs" << endl;
    ui2->setTextures(textures);

    cout << "afterBefore" << endl;

    ui->updateTBO(dimensions, 2, 30);

    cout << "before for 2" << endl;

    for(int i=0;i<84;i++){
        this->dimensions[i] = dimensions[i];
    }

    cout << "after for 2" << endl;
}

GLuint TextureSelector::getActiveTex() const
{
    return activeTex;
}

void TextureSelector::setActiveTex(const GLuint &value)
{
    activeTex = value;
}

bool TextureSelector::getIsRemoved() const
{
    return isRemoved;
}

void TextureSelector::setIsRemoved(bool value)
{
    isRemoved = value;
}

TextureSelector::TextureSelector()
{

}

void TextureSelector::setMouse(GLfloat mx, GLfloat my, bool clicked)
{
    this->mx = mx;
    this->my = my;
    this->clicked = clicked;
}

void TextureSelector::initUI(vector<string> &folderContent, GLuint texture){
    ui = new UI();
    ui2 = new UI();
    string filename = "UI/textureSelector.bin";
    string filename2 = "UI/textureLocator.bin";
    ui->init(filename);
    ui2->init(filename2);

    folderListing = new TextWriter();

    cout << "List of files in folder Textures:" << endl << endl;
    for(int i=0;i<folderContent.size();i++){
        cout << folderContent[i].c_str() << endl;
        if(i < scrollLength){
            folderListing->addText(folderContent[i],0.9,0.9-i*0.1);
        }
    }
    if(folderContent.size() < scrollLength){
        for(int i=folderContent.size();i<scrollLength;i++){
            folderListing->addText("",0.9,0.9-i*0.1);
        }
    }
    cout << endl << "End of list!" << endl << endl;

    ui->updateVBOs();
    ui2->updateVBOs();

    this->folderContent = folderContent;
    this->textTexture = texture;
}

void TextureSelector::drawUI(GLuint program, GLuint width, GLuint height)
{
    GLfloat mx = (((GLfloat)(this->mx)/(GLfloat)(width)-0.5)*2.0);
    GLfloat my = (((GLfloat)(this->my)/(GLfloat)(height)-0.5)*2.0);

    GLint LID = 0;
    switch(activeUI){
        case 0:{
            LID = ui->controller(mx, my);
            if(LID != -1){
                ui->update(LID, true);
            }else{
                ui->update(LID, false);
            }
            ui->draw(program);
            ui->update(LID, false);

            for(int i=0;i<28;i++){
                if(LID == i+2){
                    if(clicked == true){
                        activeTex = i;
                        ui->setTexture(33, i);
                        ui->updateTBO(dimensions, 33, 34, activeTex);
                    }
                }
            }

            if(isRemoved == true){//after removing texture the dimensions must be adjusted
                ui->updateTBO(dimensions, 33, 34, activeTex);
                isRemoved = false;
            }

            if(LID == 34){//select add texture menu
                if(clicked == true){
                    activeUI = 1;
                }
            }
            if(LID == 35){//remove texture
                if(clicked == true){
                    bool trigger = false;
                    ifstream remover;
                    remover.open("texture.data");

                    GLuint reNum;
                    remover >> reNum;
                    vector<string> reTomb;
                    for(int i=0;i<reNum;i++){
                        string temp;
                        remover >> temp;
                        if(activeTex != i){
                            reTomb.push_back(temp);
                        }else{
                            trigger = true;
                        }
                    }

                    remover.close();

                    if(trigger){
                        reNum--;
                        isRemoved = true;
                    }

                    ofstream writer;
                    writer.open("texture.data");

                    writer << reNum << endl;
                    for(int i=0;i<reNum;i++){
                        writer << reTomb[i] << endl;
                    }

                    writer.close();
                }
            }
            if(LID == 36){//change if texture animated

            }

            break;
        }
        case 1:{
            scrollPageNum = (GLuint)(folderContent.size() / scrollLength) + 1;

            LID = ui2->controller(mx, my);

            if(actualScrollPage != scrollPageNum - 1){
                for(int i=0;i<scrollLength;i++){
                    if(LID == i+1){
                        if(clicked == true){
                            if(scrollID == i+1){
                                isUpdate = true;
                                if(scrollID == 1){
                                    direction = 2;
                                }
                                if(scrollID == 2){
                                    direction = 1;
                                }
                                if(scrollID > 2){
                                    if(scrollID-1 < folderCount){
                                        direction = 0;
                                    }else{
                                        isUpdate = false;
                                    }
                                }
                            }
                            scrollID = i+1;
                        }
                    }
                }
            }else{
                GLuint lastLength = folderContent.size() - scrollLength*(scrollPageNum-1);
                for(int i=0;i<lastLength;i++){
                    if(LID == i+1){
                        if(clicked == true){
                            if(scrollID == i+1){
                                isUpdate = true;
                                if(scrollID == 1){
                                    direction = 2;
                                }
                                if(scrollID == 2){
                                    direction = 1;
                                }
                                if(scrollID > 2){
                                    if(scrollID-1 < folderCount){
                                        direction = 0;
                                    }else{
                                        isUpdate = false;
                                    }
                                }
                            }
                            scrollID = i+1;
                        }
                    }
                }
            }

            for(int i=0;i<scrollLength;i++){
                if(scrollID != i+1){
                    ui2->update(i+1, false);
                }
            }

            if(actualScrollPage != scrollPageNum - 1){
                if(LID != -1){
                    ui2->update(LID, true);
                }else{
                    ui2->update(LID, false);
                }
            }else{
                GLuint lastLength = folderContent.size() - scrollLength*(scrollPageNum-1);
                if(LID != -1 && (LID < lastLength + 1 || LID > scrollLength)){
                    ui2->update(LID, true);
                }else{
                    ui2->update(LID, false);
                }
            }

            if(actualScrollPage == scrollPageNum - 1){
                GLuint lastLength = folderContent.size() - scrollLength*(scrollPageNum-1);
                if(scrollID > lastLength){
                    scrollID = lastLength;
                }
            }

            for(int i=0;i<scrollLength;i++){
                if(scrollID == i+1){
                    ui2->update(i+1, true);
                }
            }

            ui2->draw(program);
            ui2->update(LID, false);
            folderListing->drawText(program, textTexture);

            if(LID == 15){//CANCEL
                if(clicked == true){
                    activeUI = 0;
                }
            }
            if(LID == 16){//OK
                if(clicked == true){
                    isSelected = true;
                    activeUI = 0;
                }
            }

            if(LID == 18){
                if(clicked == true){
                    if(actualScrollPage > 0){
                        actualScrollPage--;
                        if(actualScrollPage != scrollPageNum-1){
                            for(int i=0;i<scrollLength;i++){
                                folderListing->updateText(i, folderContent[i + actualScrollPage*scrollLength]);
                            }
                        }
                    }
                }
            }
            if(LID == 19){
                if(clicked == true){
                    if(actualScrollPage < scrollPageNum-1){
                        actualScrollPage++;
                        if(actualScrollPage != scrollPageNum-1){
                            for(int i=0;i<scrollLength;i++){
                                folderListing->updateText(i, folderContent[i + actualScrollPage*scrollLength]);
                            }
                        }else{
                            GLuint lastLength = folderContent.size() - scrollLength*(scrollPageNum-1);

                            for(int i=0;i<lastLength;i++){
                                folderListing->updateText(i, folderContent[i + actualScrollPage*scrollLength]);
                            }
                            for(int i=lastLength;i<scrollLength;i++){
                                folderListing->updateText(i, "");
                            }
                        }
                    }
                }
            }

            break;
        }
    }
}

void TextureSelector::updateText(vector<string> &folderContent)
{
    this->folderContent = folderContent;
    GLuint lastLength = folderContent.size() - scrollLength*(scrollPageNum-1);

    for(int i=0;i<lastLength;i++){
        folderListing->updateText(i, folderContent[i + actualScrollPage*scrollLength]);
    }
    for(int i=lastLength;i<scrollLength;i++){
        folderListing->updateText(i, "");
    }
}

bool TextureSelector::getIsUpdate()
{
    return isUpdate;
}

void TextureSelector::setIsUpdate(bool value)
{
    isUpdate = value;
}

GLint TextureSelector::getDirection()
{
    return direction;
}

void TextureSelector::setDirection(GLint value)
{
    direction = value;
}

string TextureSelector::getNewPath()
{
    GLuint ID = scrollID-1 + scrollLength*actualScrollPage;
    newPath = folderContent[ID];

    actualScrollPage = 0;
    scrollID = 1;
    return newPath;
}
