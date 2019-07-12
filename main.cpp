#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include <chrono>
#ifdef _WIN32_WINNT
#include "windows.h"
#else
#include <experimental/filesystem>
using namespace std::experimental;
#endif
#include <algorithm>

//#include "GL/glus.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include "textwriter.h"
#include "types.h"
#include "textureselector.h"
#include "generatecppclass.h"

using namespace std;

GLfloat projectionMatrix[16];
GLfloat lmodelMatrix[16];
GLfloat viewMatrix[16];
GLfloat lMVPMatrix[16];

GLuint shaderProgram;
GLuint shaderProgramTitle;

GLFWwindow * window;
GLFWwindow * windowTitle;

GLuint awidth = 1152;
GLuint aheight = 648;
GLuint awidthTitle = 1152;
GLuint aheightTitle = 40;

static GLuint g_vao;
static GLuint g_vao2;

GLuint VBO;
GLuint CBO;
GLuint TBO;

vector<GLfloat> vbo;
vector<GLubyte> cbo;
vector<GLfloat> tbo;


GLuint blockNumTitle = 0;

GLuint VBOT;
GLuint CBOT;
GLuint TBOT;

vector<GLfloat> vbot;
vector<GLubyte> cbot;
vector<GLfloat> tbot;

GLuint titleEditorButton;
GLuint titleBar;
GLuint titleAddButton;
GLuint titleTexButton;
GLuint titleNodeButton;
GLuint titleArrowL;
GLuint titleArrowR;
GLuint titleButton1;
GLuint titleButton2;
GLuint titleButton3;


GLuint ColorVBO;
GLuint ColorCBO;

vector<GLfloat> colorvbo;
vector<GLubyte> colorcbo;


GLuint AIMVBO;
GLuint AIMCBO;

vector<GLfloat> aimvbo;
vector<GLubyte> aimcbo;


GLuint RacsVBO;
GLuint RacsCBO;

vector<GLfloat> racsvbo;
vector<GLubyte> racscbo;


//CAM-------
GLfloat POSX = 0;
GLfloat POSY = 0;
GLfloat POSZ = 0;
GLint scrollprev = 0;
GLfloat MX;
GLfloat MY;
GLfloat MXTitle;
GLfloat MYTitle;
GLfloat szogx;
GLfloat szogy;


GLint wtx = 0;
GLint wty = 0;
GLint movedX = 0;
GLint movedY = 0;
//----------

//KEYS------
bool BOOL_W = false;
bool BOOL_S = false;
bool BOOL_A = false;
bool BOOL_D = false;

bool BOOL_UP = false;
bool BOOL_DOWN = false;
bool BOOL_LEFT = false;
bool BOOL_RIGHT = false;

bool BOOL_SHIFT = false;
bool BOOL_CONTROL = false;

bool BOOL_LEFTCLICK = false;
bool BOOL_RIGHTCLICK = false;
//----------

bool isUL = false;
bool isRacs = false;


bool triggerOnce = false;
bool testmode = false;

//+create menu slots
GLint actualMenu = 0;
GLint numMenu = 0; //existing num
//+create empty save file for load function to work properly
//+store data about how many menus are available
//+create new project (ctrl + F1)
//+save/load individual slots separately
//+after switching to new tab auto-load (should be fast enough to not be annoying)

std::vector<struct block> blockList;
GLuint blockNum = 0;
GLuint maxBlockNum = 0;
GLuint actualBlockNum = 0;

GLfloat Globalr = 0.0;
GLfloat Globalg = 0.0;
GLfloat Globalb = 0.0;

GLfloat selectedR = 255.0;
GLfloat selectedG = 0.0;
GLfloat selectedB = 0.0;

GLfloat whiteblack = 0.0;

GLuint fpsNumber = 0;

#include <sys/types.h>
#include <dirent.h>

vector<string> folderContent;
string foldername;
string actualPath = "textures/";
string startPath = "textures/";
vector<string> prevPath;

GLuint textTexture = 0;

bool isTexturing = false;

TextureSelector * textureSelector;
bool isTextureSelector = false;

TextWriter * blockMode = NULL;
TextWriter * blockMode2 = NULL;
string blockModeTexts[10];

struct RGB{
    GLubyte r;
    GLubyte g;
    GLubyte b;
};
RGB copyColor;
struct block copyBlock;

//texturing menu
GLuint actualTexNum = 0;
GLuint texNum = 0;
GLuint texNumMax = 84;//28*3

GLuint textures[28*3];
struct dimension dimensions[28*3];
struct dimension lastDimensions;
string texPaths[28*3];
//--------------

//+all menu's blockList should be stored in memory for node editor only
//node editor saves/loads
std::vector<struct block> * nodeBlockList = NULL;
//node editor displays the nodes
//node editor interactable with mouse
//node editor shows elements that can be used to modify connections
//node editor can be used to open/close menus with other menus
//node editor lets you change the filename on each part of the menus

bool isNodeEditor = false;


typedef union PixelInfo
{
    std::uint32_t Colour;
    struct
    {
        std::uint8_t R, G, B, A;
    };
} *PPixelInfo;

class Tga
{
private:
    std::vector<std::uint8_t> Pixels;
    bool ImageCompressed;
    std::uint32_t width, height, size, BitsPerPixel;

public:
    Tga(const char* FilePath);
    std::vector<std::uint8_t> GetPixels() {return this->Pixels;}
    std::uint32_t GetWidth() const {return this->width;}
    std::uint32_t GetHeight() const {return this->height;}
    bool HasAlphaChannel() {return BitsPerPixel == 32;}
};

Tga::Tga(const char* FilePath)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()){throw std::invalid_argument("File Not Found.");}

    std::uint8_t Header[18] = {0};
    std::vector<std::uint8_t> ImageData;
    static std::uint8_t DeCompressed[12] = {0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    static std::uint8_t IsCompressed[12] = {0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

    if (!std::memcmp(DeCompressed, &Header, sizeof(DeCompressed)))
    {
        BitsPerPixel = Header[16];
        width  = Header[13] * 256 + Header[12];
        height = Header[15] * 256 + Header[14];
        size  = ((width * BitsPerPixel + 31) / 32) * 4 * height;

        if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
        {
            hFile.close();
            throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        ImageData.resize(size);
        ImageCompressed = false;
        hFile.read(reinterpret_cast<char*>(ImageData.data()), size);
    }
    else if (!std::memcmp(IsCompressed, &Header, sizeof(IsCompressed)))
    {
        BitsPerPixel = Header[16];
        width  = Header[13] * 256 + Header[12];
        height = Header[15] * 256 + Header[14];
        size  = ((width * BitsPerPixel + 31) / 32) * 4 * height;

        if ((BitsPerPixel != 24) && (BitsPerPixel != 32))
        {
            hFile.close();
            throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        PixelInfo Pixel = {0};
        int CurrentByte = 0;
        std::size_t CurrentPixel = 0;
        ImageCompressed = true;
        std::uint8_t ChunkHeader = {0};
        int BytesPerPixel = (BitsPerPixel / 8);
        ImageData.resize(width * height * sizeof(PixelInfo));

        do
        {
            hFile.read(reinterpret_cast<char*>(&ChunkHeader), sizeof(ChunkHeader));

            if(ChunkHeader < 128)
            {
                ++ChunkHeader;
                for(int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
                {
                    hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

                    ImageData[CurrentByte++] = Pixel.B;
                    ImageData[CurrentByte++] = Pixel.G;
                    ImageData[CurrentByte++] = Pixel.R;
                    if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
                }
            }
            else
            {
                ChunkHeader -= 127;
                hFile.read(reinterpret_cast<char*>(&Pixel), BytesPerPixel);

                for(int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
                {
                    ImageData[CurrentByte++] = Pixel.B;
                    ImageData[CurrentByte++] = Pixel.G;
                    ImageData[CurrentByte++] = Pixel.R;
                    if (BitsPerPixel > 24) ImageData[CurrentByte++] = Pixel.A;
                }
            }
        } while(CurrentPixel < (width * height));
    }
    else
    {
        hFile.close();
        throw std::invalid_argument("Invalid File Format. Required: 24 or 32 Bit TGA File.");
    }

    hFile.close();
    this->Pixels = ImageData;
}


GLuint loadTexture(string path){
    GLuint texture;

    cout << "TGA" << std::endl;
    Tga info = Tga(path.c_str());
    cout << "TGA 2" << std::endl;

    //glusImageLoadTga(path.c_str(), &image);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float colour[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 16);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    cout << "Before teximage" << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, info.GetWidth(), info.GetHeight(), 0, info.HasAlphaChannel() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, info.GetPixels().data());
    cout << "After teximage" << std::endl;

    //glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glGenerateMipmap(GL_TEXTURE_2D);

    if(info.GetWidth() > info.GetHeight()){
        lastDimensions.x = (GLfloat)(info.GetWidth()) / (GLfloat)(info.GetWidth());
        lastDimensions.y = (GLfloat)(info.GetWidth()) / (GLfloat)(info.GetHeight());
    }else{
        lastDimensions.x = (GLfloat)(info.GetHeight()) / (GLfloat)(info.GetWidth());
        lastDimensions.y = (GLfloat)(info.GetHeight()) / (GLfloat)(info.GetHeight());
    }

    cout << lastDimensions.x << " , " << lastDimensions.y << endl;

    //glusImageDestroyTga(&image);

    return texture;
}

void saveTextures(){
    //save texture data
    ofstream file;
    file.open("texture.data");

    file << texNum << endl;
    for(int i=0;i<texNum;i++){
        file << texPaths[i] << endl;
    }

    file.close();
}

void loadTextures(){
    glfwMakeContextCurrent(window);
    //load texture data
    ifstream file;
    file.open("texture.data");

    file >> texNum;
    for(int i=0;i<texNum;i++){
        file >> texPaths[i];
    }

    file.close();

    for(int i=0;i<texNum;i++){
        textures[i] = loadTexture(texPaths[i]);

        dimensions[i].x = lastDimensions.x;
        dimensions[i].y = lastDimensions.y;
    }

    cout << "setTextures...." << std::endl;
    textureSelector->setTextures(textures, dimensions);
    cout << "  SET!!!!" << std::endl;
}

void updateVBOs(){
    glfwMakeContextCurrent(window);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vbo.size() * sizeof(GLfloat), (GLfloat*) vbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cbo.size() * sizeof(GLubyte), (GLubyte*) cbo.data());
}

void saveUIpro(){
    ofstream file;
    file.open("project.UIpro");

    file << numMenu << endl;

    file.close();
}

void loadUIpro(){
    ifstream file;
    file.open("project.UIpro");

    file >> numMenu;

    file.close();
}

void createUI(string index){
    glfwMakeContextCurrent(window);
    ofstream myFile;
    stringstream filename;
    filename.str("");
    filename << "savedUI" << index << ".bin";
    myFile.open(filename.str().c_str(), ios::out | ios::binary);

    GLuint emptyBlockNum = 0;
    myFile.write((const char*)(&emptyBlockNum), sizeof(emptyBlockNum));

    myFile.close();
}

void saveUI(string index){
    glfwMakeContextCurrent(window);
    ofstream myFile;
    stringstream filename;
    filename.str("");
    filename << "savedUI" << index << ".bin";
    myFile.open(filename.str().c_str(), ios::out | ios::binary);

    myFile.write((const char*)(&blockNum), sizeof(blockNum));
    for(int i=0;i<blockNum;i++){
        myFile.write((const char*)(&blockList[i].ID), sizeof(blockList[i].ID));
        myFile.write(reinterpret_cast<const char*>(&blockList[i].ULx), sizeof(blockList[i].ULx));
        myFile.write(reinterpret_cast<const char*>(&blockList[i].ULy), sizeof(blockList[i].ULy));
        myFile.write(reinterpret_cast<const char*>(&blockList[i].BRx), sizeof(blockList[i].BRx));
        myFile.write(reinterpret_cast<const char*>(&blockList[i].BRy), sizeof(blockList[i].BRy));
        myFile.write(reinterpret_cast<const char*>(&blockList[i].z), sizeof(blockList[i].z));
        myFile.write((const char*)(&blockList[i].r), sizeof(blockList[i].r));
        myFile.write((const char*)(&blockList[i].g), sizeof(blockList[i].g));
        myFile.write((const char*)(&blockList[i].b), sizeof(blockList[i].b));
        myFile.write((const char*)(&blockList[i].texID), sizeof(blockList[i].texID));
        myFile.write((const char*)(&blockList[i].isHover), sizeof(blockList[i].isHover));
        myFile.write((const char*)(&blockList[i].isClicked), sizeof(blockList[i].isClicked));
    }

    myFile.close();

    stringstream fileName;
    stringstream className;
    fileName << "savedUI" << index;
    className << "SavedUI" << index;
    GenerateCppClass(blockList, fileName.str(), className.str());

    /*ofstream abcData;
    stringstream abcFileName;
    abcFileName.str("");
    abcFileName << "savedABC.bin";
    abcData.open(abcFileName.str().c_str(), ios::out | ios::binary);

    abcData.write((const char*)(&blockNum), sizeof(blockNum));
    for(int i=1;i<blockNum;i++){
        abcData.write(reinterpret_cast<const char*>(&blockList[i].ULx), sizeof(blockList[i].ULx));
        abcData.write(reinterpret_cast<const char*>(&blockList[i].ULy), sizeof(blockList[i].ULy));
        abcData.write(reinterpret_cast<const char*>(&blockList[i].BRx), sizeof(blockList[i].BRx));
        abcData.write(reinterpret_cast<const char*>(&blockList[i].BRy), sizeof(blockList[i].BRy));
    }

    abcData.close();*/
}

void loadUI(string index){
    glfwMakeContextCurrent(window);
    actualBlockNum = 0;
    blockList.erase(blockList.begin(), blockList.end());
    for(int i=0;i<maxBlockNum*18;i++){
        vbo[i] = 0;
    }
    for(int i=0;i<maxBlockNum*24;i++){
        cbo[i] = 0;
    }

    ifstream myFile;
    stringstream filename;
    filename.str("");
    filename << "savedUI" << index << ".bin";
    myFile.open(filename.str().c_str(), ios::in | ios::binary);

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

    updateVBOs();
}

void init_deinit_NodeEditor(){
    glfwMakeContextCurrent(window);
    isNodeEditor = !isNodeEditor;

    if(isNodeEditor){
        nodeBlockList = new vector<struct block>[blockNum];

        for(int j=0;j<numMenu;j++){
            ifstream myFile;
            stringstream filename;
            filename.str("");
            filename << "savedUI" << j << ".bin";
            myFile.open(filename.str().c_str(), ios::in | ios::binary);

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

                nodeBlockList[j].push_back(newBlock);
            }
        }

        actualBlockNum = 0;

        //load node data
    }else{
        //save node data

        for(int i=0;i<blockNum;i++){
            nodeBlockList[i].clear();
        }
        delete [] nodeBlockList;

        actualBlockNum = 0;

        string idx = to_string(actualMenu);
        loadUI(idx);
    }
}

bool compareFunction (std::string a, std::string b) {return a<b;}

void read_directory(const std::string& name)
{
    glfwMakeContextCurrent(window);
    GLuint folderCount = 0;
    folderContent.erase(folderContent.begin(),folderContent.end());

    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        stringstream fileName;
        fileName.str("");
        fileName << name;
        fileName << dp->d_name;

        //cout << fileName.str() << ": " << filesystem::is_directory(fileName.str().c_str()) << endl;

#ifdef _WIN32_WINNT
        if(GetFileAttributesA(fileName.str().c_str()) == FILE_ATTRIBUTE_DIRECTORY){
            folderContent.push_back(dp->d_name);
            folderCount++;
        }
#else
        if(filesystem::is_directory(fileName.str().c_str())){
            folderContent.push_back(dp->d_name);
            folderCount++;
        }
#endif
    }
    closedir(dirp);

    std::sort(folderContent.begin(), folderContent.end(), compareFunction);

    std::vector<std::string> filenamesLocal;

    dirp = opendir(name.c_str());
    while ((dp = readdir(dirp)) != NULL) {
        stringstream fileName;
        fileName.str("");
        fileName << name;
        fileName << dp->d_name;

        //cout << fileName.str() << ": " << filesystem::is_directory(fileName.str().c_str()) << endl;
#ifdef _WIN32_WINNT
        if(GetFileAttributesA(fileName.str().c_str()) != FILE_ATTRIBUTE_DIRECTORY){
            bool isTGA = true;
            string len = dp->d_name;
            string ext = ".tga";
            for(int i=len.size()-1;i>len.size()-5;i--){
                cout << len[i] << " - " << ext[i - (len.size()-4)] << endl;
                if(len[i] != ext[i - (len.size()-4)]){
                    isTGA = false;
                }
            }
            if(isTGA){
                filenamesLocal.push_back(dp->d_name);
            }
        }
#else
        if(!filesystem::is_directory(fileName.str().c_str())){
            bool isTGA = true;
            string len = dp->d_name;
            string ext = ".tga";
            for(int i=len.size()-1;i>len.size()-5;i--){
                cout << len[i] << " - " << ext[i - (len.size()-4)] << endl;
                if(len[i] != ext[i - (len.size()-4)]){
                    isTGA = false;
                }
            }
            if(isTGA){
                filenamesLocal.push_back(dp->d_name);
            }
        }
#endif
    }
    closedir(dirp);

    std::sort(filenamesLocal.begin(), filenamesLocal.end(), compareFunction);
    for(auto localString : filenamesLocal)
        folderContent.push_back(localString);

    textureSelector->setFolderCount(folderCount);
}

void rounder(GLfloat &number){
    GLint numkerek = number*100;
    number = numkerek/100.0f;
}

void initTextureSelector(){
    glfwMakeContextCurrent(window);
    textureSelector->initUI(folderContent, textTexture);
}

void initBlockModeText(){
    glfwMakeContextCurrent(window);
    blockModeTexts[0] = "BackGround";
    blockModeTexts[1] = "Button";
    blockModeTexts[2] = "ScrollList";
    blockModeTexts[3] = "ScrollBar";
    blockModeTexts[4] = "ScrollArea";
    blockModeTexts[5] = "TextBox";
    blockModeTexts[6] = "Icon"; //clickable
    blockModeTexts[7] = "Empty";
    blockModeTexts[8] = "Empty";
    blockModeTexts[9] = "Empty";


    blockMode = new TextWriter();
    blockMode->addText(blockModeTexts[0],  0.95, 0.9);
    blockMode2 = new TextWriter();
    blockMode2->addText(blockModeTexts[0], -0.70, 0.9);
}

void drawBlockModeText(){
    glfwMakeContextCurrent(window);
    if(blockNum > 0){
        if(MX < awidth/2){
            blockMode->updateText(0, blockModeTexts[blockList[actualBlockNum].ID]);
            blockMode->drawText(shaderProgram, textTexture);
        }else{
            blockMode2->updateText(0, blockModeTexts[blockList[actualBlockNum].ID]);
            blockMode2->drawText(shaderProgram, textTexture);
        }
    }
}

void initVBOs(){
    glfwMakeContextCurrent(window);
    textTexture = loadTexture("textures/abc.tga");

    textureSelector = new TextureSelector();

    read_directory(actualPath);

    initTextureSelector();
    initBlockModeText();

    maxBlockNum = 500;

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

    for(int i=0;i<6;i++){
        for(int j=0;j<1;j++){
            colorvbo.push_back(i/6.0);//x1.0
            colorvbo.push_back(1.0 - j/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back((i+1)/6.0);//x1.0
            colorvbo.push_back(1.0 - j/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back(i/6.0);//x1.0
            colorvbo.push_back(1.0 - (j+1)/12.0);//x0.5
            colorvbo.push_back(0.5);


            colorvbo.push_back((i+1)/6.0);//x1.0
            colorvbo.push_back(1.0 - j/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back((i+1)/6.0);//x1.0
            colorvbo.push_back(1.0 - (j+1)/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back(i/6.0);//x1.0
            colorvbo.push_back(1.0 - (j+1)/12.0);//x0.5
            colorvbo.push_back(0.5);

            if(j==0){
                if(i==0){
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);


                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
                if(i==1){
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
                if(i==2){
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
                if(i==3){
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
                if(i==4){
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
                if(i==5){
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
            }
            if(j==1){
                if(i==0){
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);


                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
                if(i==1){
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
                if(i==2){
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
                if(i==3){
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
                if(i==4){
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
                if(i==5){
                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
            }
            /*if(j==0){
                for(int k=0;k<6;k++){
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
            }
            if(j==2){
                for(int k=0;k<6;k++){
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);
                }
            }*/
        }
    }


    for(int i=0;i<2;i++){
        for(int j=0;j<1;j++){
            colorvbo.push_back(-1.0 + i/2.0);//x1.0
            colorvbo.push_back(1.0 - j/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back(-1.0 + (i+1)/2.0);//x1.0
            colorvbo.push_back(1.0 - j/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back(-1.0 + i/2.0);//x1.0
            colorvbo.push_back(1.0 - (j+1)/12.0);//x0.5
            colorvbo.push_back(0.5);


            colorvbo.push_back(-1.0 + (i+1)/2.0);//x1.0
            colorvbo.push_back(1.0 - j/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back(-1.0 + (i+1)/2.0);//x1.0
            colorvbo.push_back(1.0 - (j+1)/12.0);//x0.5
            colorvbo.push_back(0.5);

            colorvbo.push_back(-1.0 + i/2.0);//x1.0
            colorvbo.push_back(1.0 - (j+1)/12.0);//x0.5
            colorvbo.push_back(0.5);

            if(j==0){
                if(i==0){
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);

                    colorcbo.push_back(selectedR);
                    colorcbo.push_back(selectedG);
                    colorcbo.push_back(selectedB);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);


                    colorcbo.push_back(selectedR);
                    colorcbo.push_back(selectedG);
                    colorcbo.push_back(selectedB);
                    colorcbo.push_back(255);

                    colorcbo.push_back(selectedR);
                    colorcbo.push_back(selectedG);
                    colorcbo.push_back(selectedB);
                    colorcbo.push_back(255);

                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                    colorcbo.push_back(255);
                }
                if(i==1){
                    colorcbo.push_back(selectedR);
                    colorcbo.push_back(selectedG);
                    colorcbo.push_back(selectedB);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(selectedR);
                    colorcbo.push_back(selectedG);
                    colorcbo.push_back(selectedB);
                    colorcbo.push_back(255);


                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(0);
                    colorcbo.push_back(255);

                    colorcbo.push_back(selectedR);
                    colorcbo.push_back(selectedG);
                    colorcbo.push_back(selectedB);
                    colorcbo.push_back(255);
                }
            }
        }
    }


    aimvbo.push_back(-1.0);
    aimvbo.push_back(0.5);
    aimvbo.push_back(0.5);

    aimvbo.push_back(1.0);
    aimvbo.push_back(0.5);
    aimvbo.push_back(0.5);

    aimvbo.push_back(0.5);
    aimvbo.push_back(-1.0);
    aimvbo.push_back(0.5);

    aimvbo.push_back(0.5);
    aimvbo.push_back(1.0);
    aimvbo.push_back(0.5);


    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);

    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);

    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);

    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);


    aimvbo.push_back(-1.0);
    aimvbo.push_back(0.5);
    aimvbo.push_back(0.5);

    aimvbo.push_back(1.0);
    aimvbo.push_back(0.5);
    aimvbo.push_back(0.5);

    aimvbo.push_back(0.5);
    aimvbo.push_back(-1.0);
    aimvbo.push_back(0.5);

    aimvbo.push_back(0.5);
    aimvbo.push_back(1.0);
    aimvbo.push_back(0.5);


    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);

    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);

    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);

    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);
    aimcbo.push_back(255.0);


    for(int i=-99;i<100;i++){
        racsvbo.push_back(i/100.0);
        racsvbo.push_back(1.0);
        racsvbo.push_back(0.6);

        racsvbo.push_back(i/100.0);
        racsvbo.push_back(-1.0);
        racsvbo.push_back(0.6);


        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        if(i % 5 != 0){
            racscbo.push_back(0.25*255);
        }else{
            racscbo.push_back(0.75*255);
        }

        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        if(i % 5 != 0){
            racscbo.push_back(0.25*255);
        }else{
            racscbo.push_back(0.75*255);
        }
    }

    for(int i=-99;i<100;i++){
        racsvbo.push_back(1.0);
        racsvbo.push_back(i/100.0);
        racsvbo.push_back(0.6);

        racsvbo.push_back(-1.0);
        racsvbo.push_back(i/100.0);
        racsvbo.push_back(0.6);


        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        if(i % 5 != 0){
            racscbo.push_back(0.25*255);
        }else{
            racscbo.push_back(0.75*255);
        }

        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        racscbo.push_back(0.5*255);
        if(i % 5 != 0){
            racscbo.push_back(0.25*255);
        }else{
            racscbo.push_back(0.75*255);
        }
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


    glGenBuffers(1, &ColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
    glBufferData(GL_ARRAY_BUFFER, colorvbo.size() * sizeof(GLfloat), (GLfloat*) colorvbo.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &ColorCBO);
    glBindBuffer(GL_ARRAY_BUFFER, ColorCBO);
    glBufferData(GL_ARRAY_BUFFER, colorcbo.size() * sizeof(GLubyte), (GLubyte*) colorcbo.data(), GL_STREAM_DRAW);


    glGenBuffers(1, &AIMVBO);
    glBindBuffer(GL_ARRAY_BUFFER, AIMVBO);
    glBufferData(GL_ARRAY_BUFFER, aimvbo.size() * sizeof(GLfloat), (GLfloat*) aimvbo.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &AIMCBO);
    glBindBuffer(GL_ARRAY_BUFFER, AIMCBO);
    glBufferData(GL_ARRAY_BUFFER, aimcbo.size() * sizeof(GLubyte), (GLubyte*) aimcbo.data(), GL_STREAM_DRAW);


    glGenBuffers(1, &RacsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, RacsVBO);
    glBufferData(GL_ARRAY_BUFFER, racsvbo.size() * sizeof(GLfloat), (GLfloat*) racsvbo.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &RacsCBO);
    glBindBuffer(GL_ARRAY_BUFFER, RacsCBO);
    glBufferData(GL_ARRAY_BUFFER, racscbo.size() * sizeof(GLubyte), (GLubyte*) racscbo.data(), GL_STREAM_DRAW);


    blockNumTitle = 9;

    cout << "Reserving " << blockNumTitle << " blocks for VBOs." << endl;

    vbot.reserve(18*blockNumTitle);
    cbot.reserve(24*blockNumTitle);
    tbot.reserve(12*blockNumTitle);

    for(int j=0;j<1;j++){
        vbot.push_back(- -1);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(- (-1 + 0.15));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(- -1);
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(- (-1 + 0.15));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(- (-1 + 0.15));
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(- -1);
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(225-30);
            cbot.push_back(245-30);
            cbot.push_back(255-30);
            cbot.push_back(255);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=1;j<2;j++){
        vbot.push_back(- (-1 + 0.15));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(- (-1 + 0.30));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(- (-1 + 0.15));
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(- (-1 + 0.30));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(- (-1 + 0.30));
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(- (-1 + 0.15));
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(225-30);
            cbot.push_back(245-30);
            cbot.push_back(255-30);
            cbot.push_back(255);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=2;j<3;j++){
        vbot.push_back(-1 + 1.70);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.55);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.70);
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(-1 + 1.55);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.55);
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.70);
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(100);
            cbot.push_back(100);
            cbot.push_back(100);
            cbot.push_back(255);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=3;j<4;j++){
        vbot.push_back(-1 + 1.55);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.50);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.55);
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(-1 + 1.50);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.50);
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.55);
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(128-30);
            cbot.push_back(128-30);
            cbot.push_back(255-30);
            cbot.push_back(192);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=4;j<5;j++){
        vbot.push_back(-1 + 1.50);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.45);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.50);
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(-1 + 1.45);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.45);
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.50);
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(128-30);
            cbot.push_back(128-30);
            cbot.push_back(255-30);
            cbot.push_back(192);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=5;j<6;j++){
        vbot.push_back(-1 + 1.45);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.40);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.45);
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(-1 + 1.40);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.40);
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.45);
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(128-30);
            cbot.push_back(128-30);
            cbot.push_back(255-30);
            cbot.push_back(192);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=6;j<7;j++){
        vbot.push_back(-1 + 0.30);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.40);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 0.30);
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(-1 + 1.40);
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-1 + 1.40);
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(-1 + 0.30);
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            cbot.push_back(128);
            cbot.push_back(128);
            cbot.push_back(255);
            cbot.push_back(192);
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }

    for(int j=7;j<maxBlockNum;j++){
        vbot.push_back(-(-1 + (0.85 + (j-7) * 0.05)*2));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-(-1 + (0.85 + (j-6) * 0.05)*2));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-(-1 + (0.85 + (j-7) * 0.05)*2));
        vbot.push_back(1);
        vbot.push_back(0);


        vbot.push_back(-(-1 + (0.85 + (j-6) * 0.05)*2));
        vbot.push_back(-1);
        vbot.push_back(0);

        vbot.push_back(-(-1 + (0.85 + (j-6) * 0.05)*2));
        vbot.push_back(1);
        vbot.push_back(0);

        vbot.push_back(-(-1 + (0.85 + (j-7) * 0.05)*2));
        vbot.push_back(1);
        vbot.push_back(0);

        for(int i=0;i<6;i++){
            if(j == 7 || j == 8){
                cbot.push_back(150-30);
                cbot.push_back(150-30);
                cbot.push_back(255-30);
                cbot.push_back(192);
            }
            if(j == 9){
                cbot.push_back(255-30);
                cbot.push_back(0);
                cbot.push_back(0);
                cbot.push_back(192);
            }
        }

        tbot.push_back(0);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(0);
        tbot.push_back(0);


        tbot.push_back(1);
        tbot.push_back(1);

        tbot.push_back(1);
        tbot.push_back(0);

        tbot.push_back(0);
        tbot.push_back(0);
    }
    glfwMakeContextCurrent(windowTitle);
    glGenVertexArrays(1, &g_vao2);
    glBindVertexArray(g_vao2);
    glClearColor(0.5, 0.5, 0.5, 0.5);

    glGenBuffers(1, &VBOT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOT);
    glBufferData(GL_ARRAY_BUFFER, vbot.size() * sizeof(GLfloat), (GLfloat*) vbot.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &CBOT);
    glBindBuffer(GL_ARRAY_BUFFER, CBOT);
    glBufferData(GL_ARRAY_BUFFER, cbot.size() * sizeof(GLubyte), (GLubyte*) cbot.data(), GL_STREAM_DRAW);

    glGenBuffers(1, &TBOT);
    glBindBuffer(GL_ARRAY_BUFFER, TBOT);
    glBufferData(GL_ARRAY_BUFFER, tbot.size() * sizeof(GLfloat), (GLfloat*) tbot.data(), GL_STREAM_DRAW);

    titleEditorButton = loadTexture("textures/Editor/editorButton.tga");
    titleBar = loadTexture("textures/Editor/bar.tga");
    titleAddButton = loadTexture("textures/Editor/addButton.tga");
    titleTexButton = loadTexture("textures/Editor/texButton.tga");
    titleNodeButton = loadTexture("textures/Editor/nodeButton.tga");
    titleArrowL = loadTexture("textures/Buttons/arrowL.tga");
    titleArrowR = loadTexture("textures/Buttons/arrowR.tga");
    titleButton1 = loadTexture("textures/Editor/button1.tga");
    titleButton2 = loadTexture("textures/Editor/button2.tga");
    titleButton3 = loadTexture("textures/Editor/button3.tga");

    glfwMakeContextCurrent(window);
}

void updateAIM(){
    glfwMakeContextCurrent(window);
    if(blockNum < 1){
        //aimvbo[0] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[1] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        //aimvbo[3] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[4] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        aimvbo[6] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        //aimvbo[7] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        aimvbo[9] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        //aimvbo[10] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);


        //aimvbo[0] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[1+12] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        //aimvbo[3] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[4+12] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        aimvbo[6+12] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        //aimvbo[7] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        aimvbo[9+12] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        //aimvbo[10] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);
    }else{
        //aimvbo[0] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[1] = blockList[actualBlockNum].ULy;

        //aimvbo[3] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[4] = blockList[actualBlockNum].ULy;

        aimvbo[6] = blockList[actualBlockNum].ULx;
        //aimvbo[7] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        aimvbo[9] = blockList[actualBlockNum].ULx;
        //aimvbo[10] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);


        //aimvbo[0] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[1+12] = blockList[actualBlockNum].BRy;

        //aimvbo[3] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
        aimvbo[4+12] = blockList[actualBlockNum].BRy;

        aimvbo[6+12] = blockList[actualBlockNum].BRx;
        //aimvbo[7] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

        aimvbo[9+12] = blockList[actualBlockNum].BRx;
        //aimvbo[10] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, AIMVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, aimvbo.size() * sizeof(GLfloat), (GLfloat*) aimvbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, AIMCBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, aimcbo.size() * sizeof(GLubyte), (GLubyte*) aimcbo.data());
}

void updateColor(){
    glfwMakeContextCurrent(window);
    colorcbo[24*6 + 4] = selectedR;
    colorcbo[24*6 + 5] = selectedG;
    colorcbo[24*6 + 6] = selectedB;
    colorcbo[24*6 + 12] = selectedR;
    colorcbo[24*6 + 13] = selectedG;
    colorcbo[24*6 + 14] = selectedB;
    colorcbo[24*6 + 16] = selectedR;
    colorcbo[24*6 + 17] = selectedG;
    colorcbo[24*6 + 18] = selectedB;

    colorcbo[24*7 + 0] = selectedR;
    colorcbo[24*7 + 1] = selectedG;
    colorcbo[24*7 + 2] = selectedB;
    colorcbo[24*7 + 8] = selectedR;
    colorcbo[24*7 + 9] = selectedG;
    colorcbo[24*7 + 10] = selectedB;
    colorcbo[24*7 + 20] = selectedR;
    colorcbo[24*7 + 21] = selectedG;
    colorcbo[24*7 + 22] = selectedB;

    glBindBuffer(GL_ARRAY_BUFFER, ColorCBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colorcbo.size() * sizeof(GLubyte), (GLubyte*) colorcbo.data());
}

void addBlock(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0){
    glfwMakeContextCurrent(window);
    cout << "Adding new block..." << endl;
    blockNum++;
    actualBlockNum = blockNum-1;

    struct block newBlock;
    newBlock.ID = 0;
    newBlock.BRx = ((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0;
    newBlock.BRy = ((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0;
    newBlock.ULx = ((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0;
    newBlock.ULy = ((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0;
    newBlock.z = 1.0;
    newBlock.r = r*255;
    newBlock.g = g*255;
    newBlock.b = b*255;
    newBlock.isHover = false;
    newBlock.isClicked = false;

    blockList.push_back(newBlock);

    GLfloat size = 1;

    vbo[actualBlockNum*18 + 0] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size);//0
    vbo[actualBlockNum*18 + 1] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size);//0
    vbo[actualBlockNum*18 + 2] = (1.0);

    vbo[actualBlockNum*18 + 3] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size);//1
    vbo[actualBlockNum*18 + 4] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size);//0
    vbo[actualBlockNum*18 + 5] = (1.0);

    vbo[actualBlockNum*18 + 6] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size);//0
    vbo[actualBlockNum*18 + 7] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size);//1
    vbo[actualBlockNum*18 + 8] = (1.0);


    vbo[actualBlockNum*18 + 9] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size);//1
    vbo[actualBlockNum*18 + 10] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size);//0
    vbo[actualBlockNum*18 + 11] = (1.0);

    vbo[actualBlockNum*18 + 12] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size);//1
    vbo[actualBlockNum*18 + 13] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size);//1
    vbo[actualBlockNum*18 + 14] = (1.0);

    vbo[actualBlockNum*18 + 15] = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size);//0
    vbo[actualBlockNum*18 + 16] = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size);//1
    vbo[actualBlockNum*18 + 17] = (1.0);


    cbo[actualBlockNum*24 + 0] = (r*255);
    cbo[actualBlockNum*24 + 1] = (g*255);
    cbo[actualBlockNum*24 + 2] = (b*255);
    cbo[actualBlockNum*24 + 3] = (255);

    cbo[actualBlockNum*24 + 4] = (r*255);
    cbo[actualBlockNum*24 + 5] = (g*255);
    cbo[actualBlockNum*24 + 6] = (b*255);
    cbo[actualBlockNum*24 + 7] = (255);

    cbo[actualBlockNum*24 + 8] = (r*255);
    cbo[actualBlockNum*24 + 9] = (g*255);
    cbo[actualBlockNum*24 + 10] = (b*255);
    cbo[actualBlockNum*24 + 11] = (255);


    cbo[actualBlockNum*24 + 12] = (r*255);
    cbo[actualBlockNum*24 + 13] = (g*255);
    cbo[actualBlockNum*24 + 14] = (b*255);
    cbo[actualBlockNum*24 + 15] = (255);

    cbo[actualBlockNum*24 + 16] = (r*255);
    cbo[actualBlockNum*24 + 17] = (g*255);
    cbo[actualBlockNum*24 + 18] = (b*255);
    cbo[actualBlockNum*24 + 19] = (255);

    cbo[actualBlockNum*24 + 20] = (r*255);
    cbo[actualBlockNum*24 + 21] = (g*255);
    cbo[actualBlockNum*24 + 22] = (b*255);
    cbo[actualBlockNum*24 + 23] = (255);

    rounder(blockList[actualBlockNum].ULx);
    rounder(blockList[actualBlockNum].ULy);
    for(int i=0;i<18;i++){
        if(i != 3 && i != 7 && i != 9 && i != 12 && i != 13 && i != 16){
            rounder(vbo[actualBlockNum*18 + i]);
        }
    }
}

void removeBlock(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0){
    glfwMakeContextCurrent(window);
    cout << "Removing block..." << endl;

    blockList.erase(blockList.begin() + actualBlockNum);

    vbo[actualBlockNum*18 + 0] = 0;//0
    vbo[actualBlockNum*18 + 1] = 0;//0
    vbo[actualBlockNum*18 + 2] = (1.0);

    vbo[actualBlockNum*18 + 3] = 0;//1
    vbo[actualBlockNum*18 + 4] = 0;//0
    vbo[actualBlockNum*18 + 5] = (1.0);

    vbo[actualBlockNum*18 + 6] = 0;//0
    vbo[actualBlockNum*18 + 7] = 0;//1
    vbo[actualBlockNum*18 + 8] = (1.0);


    vbo[actualBlockNum*18 + 9] = 0;//1
    vbo[actualBlockNum*18 + 10] = 0;//0
    vbo[actualBlockNum*18 + 11] = (1.0);

    vbo[actualBlockNum*18 + 12] = 0;//1
    vbo[actualBlockNum*18 + 13] = 0;//1
    vbo[actualBlockNum*18 + 14] = (1.0);

    vbo[actualBlockNum*18 + 15] = 0;//0
    vbo[actualBlockNum*18 + 16] = 0;//1
    vbo[actualBlockNum*18 + 17] = (1.0);


    cbo[actualBlockNum*24 + 0] = (r*255);
    cbo[actualBlockNum*24 + 1] = (g*255);
    cbo[actualBlockNum*24 + 2] = (b*255);
    cbo[actualBlockNum*24 + 3] = (255);

    cbo[actualBlockNum*24 + 4] = (r*255);
    cbo[actualBlockNum*24 + 5] = (g*255);
    cbo[actualBlockNum*24 + 6] = (b*255);
    cbo[actualBlockNum*24 + 7] = (255);

    cbo[actualBlockNum*24 + 8] = (r*255);
    cbo[actualBlockNum*24 + 9] = (g*255);
    cbo[actualBlockNum*24 + 10] = (b*255);
    cbo[actualBlockNum*24 + 11] = (255);


    cbo[actualBlockNum*24 + 12] = (r*255);
    cbo[actualBlockNum*24 + 13] = (g*255);
    cbo[actualBlockNum*24 + 14] = (b*255);
    cbo[actualBlockNum*24 + 15] = (255);

    cbo[actualBlockNum*24 + 16] = (r*255);
    cbo[actualBlockNum*24 + 17] = (g*255);
    cbo[actualBlockNum*24 + 18] = (b*255);
    cbo[actualBlockNum*24 + 19] = (255);

    cbo[actualBlockNum*24 + 20] = (r*255);
    cbo[actualBlockNum*24 + 21] = (g*255);
    cbo[actualBlockNum*24 + 22] = (b*255);
    cbo[actualBlockNum*24 + 23] = (255);

    vbo.erase(vbo.begin() + actualBlockNum*18, vbo.begin() + actualBlockNum*18 + 18);
    cbo.erase(cbo.begin() + actualBlockNum*24, cbo.begin() + actualBlockNum*24 + 24);
    for(int i=0; i<6; i++){
        vbo.push_back(0.0);
        vbo.push_back(0.0);
        vbo.push_back(0.0);

        cbo.push_back(0);
        cbo.push_back(0);
        cbo.push_back(0);
        cbo.push_back(0);
    }

    if(blockNum > 0){
        blockNum--;
    }
    if(actualBlockNum >= blockNum && blockNum > 0){
        actualBlockNum = blockNum-1;
    }
}

void updateBlock(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0){
    glfwMakeContextCurrent(window);
    cout << "Updating new block..." << endl;
    //18 egy block mérete (vbo)
    //24 egy block color mérete (cbo)

    GLfloat size = 1;

    blockList[actualBlockNum].BRx = ((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size;
    blockList[actualBlockNum].BRy = ((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size;

    vbo[actualBlockNum*18 + 3] = ((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size;
    vbo[actualBlockNum*18 + 7] = ((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size;
    vbo[actualBlockNum*18 + 9] = ((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size;
    vbo[actualBlockNum*18 + 12] = ((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0*size;
    vbo[actualBlockNum*18 + 13] = ((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size;
    vbo[actualBlockNum*18 + 16] = ((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0*size;

    for(int i=0;i<6;i++){
        cbo[actualBlockNum*24 + 0 + i*4] = (GLubyte)(r*255);
        cbo[actualBlockNum*24 + 1 + i*4] = (GLubyte)(g*255);
        cbo[actualBlockNum*24 + 2 + i*4] = (GLubyte)(b*255);
    }

    //round----------
    rounder(blockList[actualBlockNum].BRx);
    rounder(blockList[actualBlockNum].BRy);
    for(int i=0;i<18;i++){
        if(i == 3 || i == 7 || i == 9 || i == 12 || i == 13 || i == 16){
            rounder(vbo[actualBlockNum*18 + i]);
        }
    }
    //---------------

    updateVBOs();
}

void updateBlockColor(GLubyte r, GLubyte g, GLubyte b){
    glfwMakeContextCurrent(window);
    cout << "Updating color of block..." << endl;
    blockList[actualBlockNum].r = r;
    blockList[actualBlockNum].g = g;
    blockList[actualBlockNum].b = b;
    for(int i=0;i<6;i++){
        cbo[actualBlockNum*24 + 0 + i*4] = (GLubyte)(r);
        cbo[actualBlockNum*24 + 1 + i*4] = (GLubyte)(g);
        cbo[actualBlockNum*24 + 2 + i*4] = (GLubyte)(b);
    }

    updateVBOs();
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

bool init(void)
{
    cout << "set clear color" << std::endl;
    glClearColor(0.25f, 0.25f, 0.25f, 0.5f);

    cout << "generate VAO" << std::endl;
    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    cout << "loading shaders...";

    shaderProgram = LoadShaders("shader/renderShader.vert.glsl", "shader/renderShader.frag.glsl");

    glfwMakeContextCurrent(windowTitle);
    shaderProgramTitle = LoadShaders("shader/renderShader.vert.glsl", "shader/renderShader.frag.glsl");
    glfwMakeContextCurrent(window);

    cout << "  loaded!!" << std::endl;

    std::cout << std::fixed;
    std::cout << std::setprecision(2);


    initVBOs();

    copyColor.r = 0;
    copyColor.g = 0;
    copyColor.b = 0;


    //floorTex = loadTexture("textures/floor1.tga");


    cout << "loading UI...";
    loadUIpro();
    cout << "  loaded!!" << std::endl;

    cout << "loading textures...";
    loadTextures();
    cout << "  loaded!!" << std::endl;

    string idx = to_string(actualMenu);

    cout << "loading UI index...";
    loadUI(idx);
    cout << "  loaded!!" << std::endl;


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    return true;
}

void reshape(GLFWwindow * window, int width, int height)
{
    glfwMakeContextCurrent(window);
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);

    awidth = width;
    aheight = height;
}

void reshapeTitle(GLFWwindow * window, int width, int height)
{
    glfwMakeContextCurrent(window);
    // Set the viewport depending on the width and height of the window.
    glViewport(0, 0, width, height);

    awidthTitle = width;
    aheightTitle = height;
}

void eventhandler(GLfloat time){
    /*if(BOOL_SHIFT){
        if(BOOL_W){
            POSX += sinf(szogy*3.1415/180.0)/2.0;
            POSZ += cosf(szogy*3.1415/180.0)/2.0;
        }
        if(BOOL_S){
            POSX -= sinf(szogy*3.1415/180.0)/2.0;
            POSZ -= cosf(szogy*3.1415/180.0)/2.0;
        }
        if(BOOL_A){
            POSX -= sinf((szogy-90)*3.1415/180.0)/2.0;
            POSZ -= cosf((szogy-90)*3.1415/180.0)/2.0;
        }
        if(BOOL_D){
            POSX += sinf((szogy-90)*3.1415/180.0)/2.0;
            POSZ += cosf((szogy-90)*3.1415/180.0)/2.0;
        }

        if(BOOL_UP){
            POSY += 0.25/1.0;
        }
        if(BOOL_DOWN){
            POSY -= 0.25/1.0;
        }
    }else{
        if(BOOL_W){
            POSX += sinf(szogy*3.1415/180.0)/15.0;
            POSZ += cosf(szogy*3.1415/180.0)/15.0;
        }
        if(BOOL_S){
            POSX -= sinf(szogy*3.1415/180.0)/15.0;
            POSZ -= cosf(szogy*3.1415/180.0)/15.0;
        }
        if(BOOL_A){
            POSX -= sinf((szogy-90)*3.1415/180.0)/15.0;
            POSZ -= cosf((szogy-90)*3.1415/180.0)/15.0;
        }
        if(BOOL_D){
            POSX += sinf((szogy-90)*3.1415/180.0)/15.0;
            POSZ += cosf((szogy-90)*3.1415/180.0)/15.0;
        }

        if(BOOL_UP){
            POSY += 0.25/20.0;
        }
        if(BOOL_DOWN){
            POSY -= 0.25/20.0;
        }
    }*/
}

GLfloat elapsedTime = 0;

void viewport()
{
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //glm::mat4 Projection = glm::perpective(glm::radians(90.0f), (float) 1280 / (float)720, 0.01f, 30.0f);

    // Or, for an ortho camera :
    glm::mat4 Projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, 0.01, 10.0); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(POSX, POSY, POSZ-1), // Camera is at (4,3,3), in World Space
        glm::vec3(POSX, POSY, POSZ), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
}

void drawUI(){
    glUseProgram(shaderProgram);

    viewport();

    //glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"MVP"), 1, GL_FALSE, lMVPMatrix);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_tex0"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));

    /*if(isTexturing == true){
        glUniform1i(glGetUniformLocation(shaderProgram,"isTextured"), 1);
    }else{
        glUniform1i(glGetUniformLocation(shaderProgram,"isTextured"), 0);
    }
    glDrawArrays(GL_TRIANGLES, 0, blockNum*6);*/

    if(isTexturing == true){
        for(int i=0;i<blockNum;i++){
            //glUniform1i(glGetUniformLocation(program,"isTextured"), 0);
            glUniform1i(glGetUniformLocation(shaderProgram,"isTextured"), 1);
            glBindTexture(GL_TEXTURE_2D, textures[blockList[i].texID]);//texturing is under development
            glDrawArrays(GL_TRIANGLES, i*6, 6);
        }
    }else{
        glUniform1i(glGetUniformLocation(shaderProgram,"isTextured"), 0);
        glDrawArrays(GL_TRIANGLES, 0, blockNum*6);
    }

    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));
}

void drawColors(){
    glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));

    glBindBuffer(GL_ARRAY_BUFFER, ColorCBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));

    /*glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_tex0"), 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));*/

    //glBindTexture(GL_TEXTURE_2D, floorTex);


    glDrawArrays(GL_TRIANGLES, 0, colorvbo.size()/3);


    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));
    //glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));
}

void drawAim(){
    glBindBuffer(GL_ARRAY_BUFFER, AIMVBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));

    glBindBuffer(GL_ARRAY_BUFFER, AIMCBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));

    /*glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_tex0"), 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));*/

    //glBindTexture(GL_TEXTURE_2D, floorTex);


    glDrawArrays(GL_LINES, 0, aimvbo.size()/3);


    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));
    //glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));
}

void drawRacs(){
    if(isRacs){
        glBindBuffer(GL_ARRAY_BUFFER, RacsVBO);
        glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));

        glBindBuffer(GL_ARRAY_BUFFER, RacsCBO);
        glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));

        /*glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glVertexAttribPointer(glGetAttribLocation(shaderProgram,"a_tex0"), 2, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));*/

        //glBindTexture(GL_TEXTURE_2D, floorTex);


        glDrawArrays(GL_LINES, 0, racsvbo.size()/3);


        glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_vertex"));
        glDisableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_color"));
        //glEnableVertexAttribArray(glGetAttribLocation(shaderProgram,"a_tex0"));
    }
}

void drawTitleBar(){
    glUseProgram(shaderProgramTitle);

    viewport();

    glBindBuffer(GL_ARRAY_BUFFER, VBOT);
    glVertexAttribPointer(glGetAttribLocation(shaderProgramTitle,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgramTitle,"a_vertex"));

    glBindBuffer(GL_ARRAY_BUFFER, CBOT);
    glVertexAttribPointer(glGetAttribLocation(shaderProgramTitle,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgramTitle,"a_color"));

    glBindBuffer(GL_ARRAY_BUFFER, TBOT);
    glVertexAttribPointer(glGetAttribLocation(shaderProgramTitle,"a_tex0"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(shaderProgramTitle,"a_tex0"));

    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleEditorButton);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleTexButton);
    glDrawArrays(GL_TRIANGLES, 6, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleNodeButton);
    glDrawArrays(GL_TRIANGLES, 12, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleArrowL);
    glDrawArrays(GL_TRIANGLES, 18, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleArrowR);
    glDrawArrays(GL_TRIANGLES, 24, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleAddButton);
    glDrawArrays(GL_TRIANGLES, 30, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleBar);
    glDrawArrays(GL_TRIANGLES, 36, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleButton1);
    glDrawArrays(GL_TRIANGLES, 42, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleButton2);
    glDrawArrays(GL_TRIANGLES, 48, 6);
    glUniform1i(glGetUniformLocation(shaderProgramTitle,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, titleButton3);
    glDrawArrays(GL_TRIANGLES, 54, 6);

    glDisableVertexAttribArray(glGetAttribLocation(shaderProgramTitle,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderProgramTitle,"a_color"));
    glDisableVertexAttribArray(glGetAttribLocation(shaderProgramTitle,"a_tex0"));
}

bool updateTitleBar(){
    // Now, the background is painted blue.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw bar here
    drawTitleBar();

    return true;
}

bool update(float time)
{
    // Now, the background is painted blue.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //eventhandling----------------
    eventhandler(time);
    //-----------------------------

    //drawing----------------------
    if(isTextureSelector){
        textureSelector->setMouse(MX, MY, BOOL_LEFTCLICK);
        textureSelector->drawUI(shaderProgram, awidth, aheight);

        if(textureSelector->getIsUpdate() == true){
            stringstream newPath;
            newPath.str("");

            switch(textureSelector->getDirection()){
                case 0:{
                    //direction (forward)
                    prevPath.push_back(actualPath);
                    string addPath = textureSelector->getNewPath();
                    newPath << actualPath << addPath << "/";
                    cout << "forward" << endl;
                    break;
                }
                case 1:{//direction (backward)
                    if(prevPath.size() > 0){//if previous folder exists
                        newPath << prevPath[prevPath.size()-1];
                        prevPath.erase(prevPath.end());
                        cout << "backword" << endl;
                    }else{
                        newPath << "textures/";
                    }
                    break;
                }
                case 2:{//direction (starting folder)
                    newPath << startPath;
                    prevPath.erase(prevPath.begin(), prevPath.end());
                    cout << "starting folder" << endl;
                    break;
                }
            }

            read_directory(newPath.str());
            actualPath = newPath.str();

            textureSelector->updateText(folderContent);

            textureSelector->setDirection(-1);
            textureSelector->setIsUpdate(false);
        }
        if(textureSelector->getIsSelected()){
            stringstream selectedFile;
            selectedFile.str("");
            selectedFile << actualPath << textureSelector->getSelected();

            textures[texNum] = loadTexture(selectedFile.str());
            texPaths[texNum] = selectedFile.str();

            texNum++;

            textureSelector->setIsSelected(false);
            textureSelector->setTextures(textures, dimensions);
            saveTextures();
            loadTextures();
        }
        if(textureSelector->getIsRemoved()){
            textures[texNum-1] = 0;

            loadTextures();
            //textures[texNum] = loadTexture("textures/backgrounds/emptyTex.tga");

            textureSelector->setTextures(textures, dimensions);
            //textureSelector->setIsRemoved(false);
        }

        BOOL_LEFTCLICK = false;
    }else{
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        drawBlockModeText();
        drawUI();

        //glfwSetCursorPos(window,awidth/2.0,aheight/2.0);

        //create windows title---------
        elapsedTime += time*1000.0f;
        if(elapsedTime >= 250){
            elapsedTime -= 250;

            fpsNumber = (GLint)(1000.0f / (time*1000.0f));
        }
        stringstream title;
        title.str("");
        title << "UI creator  [FPS: " << fpsNumber << "]";
        title << " , [Menu: " << actualMenu+1 << "/" << numMenu+1 << "]";

        if(blockNum > 0){
            title << setprecision(2) << fixed << " , ActualBlock[ULx: " << blockList[actualBlockNum].ULx <<
                                                             " , ULy: " << blockList[actualBlockNum].ULy <<
                                                             " , BRx: " << blockList[actualBlockNum].BRx <<
                                                             " , BRy: " << blockList[actualBlockNum].BRy <<
                                                             " , z: " << blockList[actualBlockNum].z << "]";
            title << " , [ID: " << blockList[actualBlockNum].ID << "] , [TexID: " << blockList[actualBlockNum].texID
                                                                << "] , [Hover: " << blockList[actualBlockNum].isHover
                                                                << "] , [Clicked: " << blockList[actualBlockNum].isClicked << "]";
        }

        glfwSetWindowTitle(window,title.str().c_str());
        //-----------------------------

        if(testmode != true){
            if(triggerOnce == true){
                addBlock(Globalr/255.0, Globalg/255.0, Globalb/255.0);
                triggerOnce = false;
            }
            if(BOOL_SHIFT != true && BOOL_LEFTCLICK == true){
                updateBlock(Globalr/255.0, Globalg/255.0, Globalb/255.0);
            }
            if(BOOL_SHIFT){
                drawColors();
                if(BOOL_LEFTCLICK){
                    if((((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0) > 0.0){
                        GLfloat percent = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
                        if(percent > 0.0/6.0 && percent < 1.0/6.0){
                            selectedR = ((percent - 0.0/6.0)*6.0 * 255 + (1.0 - (percent - 0.0/6.0)*6.0) * 255);
                            selectedG = ((percent - 0.0/6.0)*6.0 * 255 + (1.0 - (percent - 0.0/6.0)*6.0) * 0);
                            selectedB = ((percent - 0.0/6.0)*6.0 * 0   + (1.0 - (percent - 0.0/6.0)*6.0) * 0);
                        }
                        if(percent > 1.0/6.0 && percent < 2.0/6.0){
                            selectedR = ((percent - 1.0/6.0)*6.0 * 0   + (1.0 - (percent - 1.0/6.0)*6.0) * 255);
                            selectedG = ((percent - 1.0/6.0)*6.0 * 255 + (1.0 - (percent - 1.0/6.0)*6.0) * 255);
                            selectedB = ((percent - 1.0/6.0)*6.0 * 0   + (1.0 - (percent - 1.0/6.0)*6.0) * 0);
                        }
                        if(percent > 2.0/6.0 && percent < 3.0/6.0){
                            selectedR = ((percent - 2.0/6.0)*6.0 * 0   + (1.0 - (percent - 2.0/6.0)*6.0) * 0);
                            selectedG = ((percent - 2.0/6.0)*6.0 * 255 + (1.0 - (percent - 2.0/6.0)*6.0) * 255);
                            selectedB = ((percent - 2.0/6.0)*6.0 * 255 + (1.0 - (percent - 2.0/6.0)*6.0) * 0);
                        }
                        if(percent > 3.0/6.0 && percent < 4.0/6.0){
                            selectedR = ((percent - 3.0/6.0)*6.0 * 0   + (1.0 - (percent - 3.0/6.0)*6.0) * 0);
                            selectedG = ((percent - 3.0/6.0)*6.0 * 0   + (1.0 - (percent - 3.0/6.0)*6.0) * 255);
                            selectedB = ((percent - 3.0/6.0)*6.0 * 255 + (1.0 - (percent - 3.0/6.0)*6.0) * 255);
                        }
                        if(percent > 4.0/6.0 && percent < 5.0/6.0){
                            selectedR = ((percent - 4.0/6.0)*6.0 * 255 + (1.0 - (percent - 4.0/6.0)*6.0) * 0);
                            selectedG = ((percent - 4.0/6.0)*6.0 * 0   + (1.0 - (percent - 4.0/6.0)*6.0) * 0);
                            selectedB = ((percent - 4.0/6.0)*6.0 * 255 + (1.0 - (percent - 4.0/6.0)*6.0) * 255);
                        }
                        if(percent > 5.0/6.0 && percent < 6.0/6.0){
                            selectedR = ((percent - 5.0/6.0)*6.0 * 255 + (1.0 - (percent - 5.0/6.0)*6.0) * 255);
                            selectedG = ((percent - 5.0/6.0)*6.0 * 0   + (1.0 - (percent - 5.0/6.0)*6.0) * 0);
                            selectedB = ((percent - 5.0/6.0)*6.0 * 0   + (1.0 - (percent - 5.0/6.0)*6.0) * 255);
                        }
                    }
                    if((((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0) < 0.0){
                        GLfloat percent = -(((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
                        if(percent < 0.5){
                            Globalr = selectedR*percent*2 + 0*(1.0-percent)*2;
                            Globalg = selectedG*percent*2 + 0*(1.0-percent)*2;
                            Globalb = selectedB*percent*2 + 0*(1.0-percent)*2;
                        }
                        if(percent > 0.5){
                            Globalr = selectedR*(1.0-(percent-0.5)*2) + 255*((percent-0.5))*2;
                            Globalg = selectedG*(1.0-(percent-0.5)*2) + 255*((percent-0.5))*2;
                            Globalb = selectedB*(1.0-(percent-0.5)*2) + 255*((percent-0.5))*2;
                        }
                    }

                    updateColor();
                    updateBlockColor(Globalr,Globalg,Globalb);
                }
            }
        }

        glUniform1i(glGetUniformLocation(shaderProgram,"isTextured"), 0);

        updateAIM();

        drawAim();
        drawRacs();
    }

    return true;
}

void terminate2(void)
{
    // No resources have to be freed in this case.
}

void modifierHandle(GLFWwindow *window, int key, int scancode, int action, int mods){

}

void keyhandle(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_LEFT_SHIFT){
            BOOL_SHIFT = true;
        }
        if(key == GLFW_KEY_LEFT_CONTROL){
            BOOL_CONTROL = true;
        }

        if(key == GLFW_KEY_W){
            BOOL_W = true;
            if(BOOL_CONTROL){
                if(blockList[actualBlockNum].ULy < 0.99){
                    blockList[actualBlockNum].ULy += 0.01;
                    blockList[actualBlockNum].BRy += 0.01;
                }

                vbo[actualBlockNum*18 + 1] = blockList[actualBlockNum].ULy;
                vbo[actualBlockNum*18 + 4] = blockList[actualBlockNum].ULy;
                vbo[actualBlockNum*18 + 10] = blockList[actualBlockNum].ULy;

                vbo[actualBlockNum*18 + 7] = blockList[actualBlockNum].BRy;
                vbo[actualBlockNum*18 + 13] = blockList[actualBlockNum].BRy;
                vbo[actualBlockNum*18 + 16] = blockList[actualBlockNum].BRy;
                updateVBOs();
            }
        }
        if(key == GLFW_KEY_S){
            BOOL_S = true;
            if(BOOL_CONTROL){
                if(blockList[actualBlockNum].BRy > -0.99){
                    blockList[actualBlockNum].ULy -= 0.01;
                    blockList[actualBlockNum].BRy -= 0.01;
                }

                vbo[actualBlockNum*18 + 1] = blockList[actualBlockNum].ULy;
                vbo[actualBlockNum*18 + 4] = blockList[actualBlockNum].ULy;
                vbo[actualBlockNum*18 + 10] = blockList[actualBlockNum].ULy;

                vbo[actualBlockNum*18 + 7] = blockList[actualBlockNum].BRy;
                vbo[actualBlockNum*18 + 13] = blockList[actualBlockNum].BRy;
                vbo[actualBlockNum*18 + 16] = blockList[actualBlockNum].BRy;
                updateVBOs();
            }
        }
        if(key == GLFW_KEY_A){
            BOOL_A = true;
            if(BOOL_CONTROL){
                if(blockList[actualBlockNum].ULx < 0.99){
                    blockList[actualBlockNum].ULx += 0.01;
                    blockList[actualBlockNum].BRx += 0.01;
                }
                vbo[actualBlockNum*18 + 0] = blockList[actualBlockNum].ULx;
                vbo[actualBlockNum*18 + 6] = blockList[actualBlockNum].ULx;
                vbo[actualBlockNum*18 + 15] = blockList[actualBlockNum].ULx;

                vbo[actualBlockNum*18 + 3] = blockList[actualBlockNum].BRx;
                vbo[actualBlockNum*18 + 9] = blockList[actualBlockNum].BRx;
                vbo[actualBlockNum*18 + 12] = blockList[actualBlockNum].BRx;

                updateVBOs();
            }
        }
        if(key == GLFW_KEY_D){
            BOOL_D = true;
            if(BOOL_CONTROL){
                if(blockList[actualBlockNum].BRx > -0.99){
                    blockList[actualBlockNum].ULx -= 0.01;
                    blockList[actualBlockNum].BRx -= 0.01;
                }
                vbo[actualBlockNum*18 + 0] = blockList[actualBlockNum].ULx;
                vbo[actualBlockNum*18 + 6] = blockList[actualBlockNum].ULx;
                vbo[actualBlockNum*18 + 15] = blockList[actualBlockNum].ULx;

                vbo[actualBlockNum*18 + 3] = blockList[actualBlockNum].BRx;
                vbo[actualBlockNum*18 + 9] = blockList[actualBlockNum].BRx;
                vbo[actualBlockNum*18 + 12] = blockList[actualBlockNum].BRx;

                updateVBOs();
            }
        }

        if(key == GLFW_KEY_UP){
            BOOL_UP = true;
            if(BOOL_CONTROL){
                if(isUL == true){
                    if(blockList[actualBlockNum].ULy < 0.99){
                        blockList[actualBlockNum].ULy += 0.01;
                    }

                    //vbo[actualBlockNum*18 + 3] = ;
                    vbo[actualBlockNum*18 + 1] = blockList[actualBlockNum].ULy;
                    //vbo[actualBlockNum*18 + 9] = ;
                    //vbo[actualBlockNum*18 + 12] = ;
                    vbo[actualBlockNum*18 + 4] = blockList[actualBlockNum].ULy;
                    vbo[actualBlockNum*18 + 10] = blockList[actualBlockNum].ULy;

                    updateVBOs();
                }else{
                    if(blockList[actualBlockNum].BRy < 0.99){
                        blockList[actualBlockNum].BRy += 0.01;
                    }

                    //vbo[actualBlockNum*18 + 3] = ;
                    vbo[actualBlockNum*18 + 7] = blockList[actualBlockNum].BRy;
                    //vbo[actualBlockNum*18 + 9] = ;
                    //vbo[actualBlockNum*18 + 12] = ;
                    vbo[actualBlockNum*18 + 13] = blockList[actualBlockNum].BRy;
                    vbo[actualBlockNum*18 + 16] = blockList[actualBlockNum].BRy;

                    updateVBOs();
                }
            }
        }
        if(key == GLFW_KEY_DOWN){
            BOOL_DOWN = true;
            if(BOOL_CONTROL){
                if(isUL == true){
                    if(blockList[actualBlockNum].ULy > -0.99){
                        blockList[actualBlockNum].ULy -= 0.01;
                    }

                    //vbo[actualBlockNum*18 + 3] = ;
                    vbo[actualBlockNum*18 + 1] = blockList[actualBlockNum].ULy;
                    //vbo[actualBlockNum*18 + 9] = ;
                    //vbo[actualBlockNum*18 + 12] = ;
                    vbo[actualBlockNum*18 + 4] = blockList[actualBlockNum].ULy;
                    vbo[actualBlockNum*18 + 10] = blockList[actualBlockNum].ULy;

                    updateVBOs();
                }else{
                    if(blockList[actualBlockNum].BRy > -0.99){
                        blockList[actualBlockNum].BRy -= 0.01;
                    }

                    //vbo[actualBlockNum*18 + 3] = ;
                    vbo[actualBlockNum*18 + 7] = blockList[actualBlockNum].BRy;
                    //vbo[actualBlockNum*18 + 9] = ;
                    //vbo[actualBlockNum*18 + 12] = ;
                    vbo[actualBlockNum*18 + 13] = blockList[actualBlockNum].BRy;
                    vbo[actualBlockNum*18 + 16] = blockList[actualBlockNum].BRy;

                    updateVBOs();
                }
            }
        }
        if(key == GLFW_KEY_RIGHT){
            BOOL_RIGHT = true;
            if(BOOL_CONTROL != true){
                if(actualBlockNum < blockNum-1){
                    actualBlockNum++;
                }
            }else{
                if(isUL == true){
                    if(blockList[actualBlockNum].ULx > -0.99){
                        blockList[actualBlockNum].ULx -= 0.01;
                    }

                    //vbo[actualBlockNum*18 + 3] = ;
                    vbo[actualBlockNum*18 + 0] = blockList[actualBlockNum].ULx;
                    //vbo[actualBlockNum*18 + 9] = ;
                    //vbo[actualBlockNum*18 + 12] = ;
                    vbo[actualBlockNum*18 + 6] = blockList[actualBlockNum].ULx;
                    vbo[actualBlockNum*18 + 15] = blockList[actualBlockNum].ULx;

                    updateVBOs();
                }else{
                    if(blockList[actualBlockNum].BRx > -0.99){
                        blockList[actualBlockNum].BRx -= 0.01;
                    }

                    vbo[actualBlockNum*18 + 3] = blockList[actualBlockNum].BRx;
                    vbo[actualBlockNum*18 + 9] = blockList[actualBlockNum].BRx;
                    vbo[actualBlockNum*18 + 12] = blockList[actualBlockNum].BRx;

                    updateVBOs();
                }
            }
        }
        if(key == GLFW_KEY_LEFT){
            BOOL_LEFT = true;
            if(BOOL_CONTROL != true){
                if(actualBlockNum > 0){
                    actualBlockNum--;
                }
            }else{
                if(isUL == true){
                    if(blockList[actualBlockNum].ULx < 0.99){
                        blockList[actualBlockNum].ULx += 0.01;
                    }

                    //vbo[actualBlockNum*18 + 3] = ;
                    vbo[actualBlockNum*18 + 0] = blockList[actualBlockNum].ULx;
                    //vbo[actualBlockNum*18 + 9] = ;
                    //vbo[actualBlockNum*18 + 12] = ;
                    vbo[actualBlockNum*18 + 6] = blockList[actualBlockNum].ULx;
                    vbo[actualBlockNum*18 + 15] = blockList[actualBlockNum].ULx;

                    updateVBOs();
                }else{
                    if(blockList[actualBlockNum].BRx < 0.99){
                        blockList[actualBlockNum].BRx += 0.01;
                    }

                    vbo[actualBlockNum*18 + 3] = blockList[actualBlockNum].BRx;
                    vbo[actualBlockNum*18 + 9] = blockList[actualBlockNum].BRx;
                    vbo[actualBlockNum*18 + 12] = blockList[actualBlockNum].BRx;

                    updateVBOs();
                }
            }
        }

        if(key == GLFW_KEY_KP_ADD){
            if(BOOL_CONTROL == false){
                blockList[actualBlockNum].z += 0.01;

                vbo[actualBlockNum*18 + 2] = blockList[actualBlockNum].z;
                vbo[actualBlockNum*18 + 5] = blockList[actualBlockNum].z;
                vbo[actualBlockNum*18 + 8] = blockList[actualBlockNum].z;
                vbo[actualBlockNum*18 + 11] = blockList[actualBlockNum].z;
                vbo[actualBlockNum*18 + 14] = blockList[actualBlockNum].z;
                vbo[actualBlockNum*18 + 17] = blockList[actualBlockNum].z;
            }else{
                for(int i=0;i<blockNum;i++){
                    blockList[i].z += 0.01;

                    vbo[i*18 + 2] = blockList[i].z;
                    vbo[i*18 + 5] = blockList[i].z;
                    vbo[i*18 + 8] = blockList[i].z;
                    vbo[i*18 + 11] = blockList[i].z;
                    vbo[i*18 + 14] = blockList[i].z;
                    vbo[i*18 + 17] = blockList[i].z;
                }
            }
            updateVBOs();
        }
        if(key == GLFW_KEY_KP_SUBTRACT){
            if(blockList[actualBlockNum].z > 0.1){
                if(BOOL_CONTROL == false){
                    blockList[actualBlockNum].z -= 0.01;

                    vbo[actualBlockNum*18 + 2] = blockList[actualBlockNum].z;
                    vbo[actualBlockNum*18 + 5] = blockList[actualBlockNum].z;
                    vbo[actualBlockNum*18 + 8] = blockList[actualBlockNum].z;
                    vbo[actualBlockNum*18 + 11] = blockList[actualBlockNum].z;
                    vbo[actualBlockNum*18 + 14] = blockList[actualBlockNum].z;
                    vbo[actualBlockNum*18 + 17] = blockList[actualBlockNum].z;
                }else{
                    for(int i=0;i<blockNum;i++){
                        blockList[i].z -= 0.01;

                        vbo[i*18 + 2] = blockList[i].z;
                        vbo[i*18 + 5] = blockList[i].z;
                        vbo[i*18 + 8] = blockList[i].z;
                        vbo[i*18 + 11] = blockList[i].z;
                        vbo[i*18 + 14] = blockList[i].z;
                        vbo[i*18 + 17] = blockList[i].z;
                    }
                }
                updateVBOs();
            }
        }
        if(key == GLFW_KEY_1){
            isRacs = !isRacs;
        }

        if(key == GLFW_KEY_F1){
            if(BOOL_CONTROL == false){
                //add new menu
                numMenu++;
                string idx = to_string(numMenu);
                createUI(idx);
                saveUIpro();
            }else{
                //create new project
                numMenu = 0;
                actualMenu = 0;
                string idx = to_string(numMenu);
                createUI(idx);
                //project file needed to work properly

                saveUIpro();
                loadUI(idx);
            }
        }
        if(key == GLFW_KEY_F2){
            //select menu -
            string idx = to_string(actualMenu);
            saveUI(idx);
            if(actualMenu > 0){
                actualMenu -= 1;
            }
            idx = to_string(actualMenu);
            loadUI(idx);
        }
        if(key == GLFW_KEY_F3){
            //select menu +
            string idx = to_string(actualMenu);
            saveUI(idx);
            if(actualMenu < numMenu){
                actualMenu += 1;
            }
            idx = to_string(actualMenu);
            loadUI(idx);
        }
        if(key == GLFW_KEY_F4){
            //remove menu
            if(numMenu > 0){
                for(int i=actualMenu;i<numMenu;i++){
                    string idx = to_string(i+1);
                    loadUI(idx);

                    idx = to_string(i);
                    saveUI(idx);
                }

                numMenu--;

                if(actualMenu > numMenu){
                    actualMenu = numMenu;
                }

                string idx = to_string(actualMenu);
                loadUI(idx);
            }
        }
        if(key == GLFW_KEY_F5){
            string idx = to_string(actualMenu);
            createUI(idx);
            loadUI(idx);
        }
        if(key == GLFW_KEY_F7){
            string idx = to_string(actualMenu);
            saveUI(idx);
            saveUIpro();
            saveTextures();
        }
        if(key == GLFW_KEY_F8){
            string idx = to_string(actualMenu);
            loadUI(idx);
        }
        if(key == GLFW_KEY_F11){
            //node editor
        }
        if(key == GLFW_KEY_F12){
            isTextureSelector = !isTextureSelector;
        }

        if(key == GLFW_KEY_TAB){
            isTexturing = !isTexturing;
        }

        if(key == GLFW_KEY_2){
            if(blockList[actualBlockNum].ID > 0){
                blockList[actualBlockNum].ID--;
            }
        }
        if(key == GLFW_KEY_3){
            if(blockList[actualBlockNum].ID < 10-1){
                blockList[actualBlockNum].ID++;
            }
        }

        if(key == GLFW_KEY_4){//texID -
            if(blockList[actualBlockNum].texID > 0){
                blockList[actualBlockNum].texID--;
            }
        }
        if(key == GLFW_KEY_5){//texID +
            if(blockList[actualBlockNum].texID < 28*3){
                blockList[actualBlockNum].texID++;
            }
        }

        if(key == GLFW_KEY_ENTER){
            blockList[actualBlockNum].texID = textureSelector->getActiveTex();
        }

        if(key == GLFW_KEY_DELETE){
            removeBlock();
            updateVBOs();
        }

        if(BOOL_SHIFT == true && key == GLFW_KEY_C){
            cout << "shift + C" << endl;
            //copy color
            copyColor.r = blockList[actualBlockNum].r;
            copyColor.g = blockList[actualBlockNum].g;
            copyColor.b = blockList[actualBlockNum].b;
        }
        if(BOOL_SHIFT == true && key == GLFW_KEY_V){
            cout << "shift + V" << endl;
            //paste color
            blockList[actualBlockNum].r = copyColor.r;
            blockList[actualBlockNum].g = copyColor.g;
            blockList[actualBlockNum].b = copyColor.b;

            for(int i=0;i<6;i++){
                cbo[actualBlockNum*24 + 0 + i*4] = (GLubyte)(copyColor.r);
                cbo[actualBlockNum*24 + 1 + i*4] = (GLubyte)(copyColor.g);
                cbo[actualBlockNum*24 + 2 + i*4] = (GLubyte)(copyColor.b);
            }
            updateBlockColor(copyColor.r, copyColor.g, copyColor.b);
        }
        if(BOOL_CONTROL == true && key == GLFW_KEY_C){
            cout << "control + C" << endl;
            //copy block
            copyBlock = blockList[actualBlockNum];
        }
        if(BOOL_CONTROL == true && key == GLFW_KEY_V){
            cout << "control + V" << endl;
            //paste block

            addBlock();
            updateBlock();

            blockList[actualBlockNum] = copyBlock;
            blockList[actualBlockNum].BRx -= 0.01;
            blockList[actualBlockNum].BRy -= 0.01;
            blockList[actualBlockNum].ULx -= 0.01;
            blockList[actualBlockNum].ULy -= 0.01;

            copyBlock.BRx = blockList[actualBlockNum].BRx;
            copyBlock.BRy = blockList[actualBlockNum].BRy;
            copyBlock.ULx = blockList[actualBlockNum].ULx;
            copyBlock.ULy = blockList[actualBlockNum].ULy;

            updateBlockColor(blockList[actualBlockNum].r,
                             blockList[actualBlockNum].g,
                             blockList[actualBlockNum].b);

            vbo[actualBlockNum*18 + 1] = blockList[actualBlockNum].ULy;
            vbo[actualBlockNum*18 + 4] = blockList[actualBlockNum].ULy;
            vbo[actualBlockNum*18 + 10] = blockList[actualBlockNum].ULy;

            vbo[actualBlockNum*18 + 7] = blockList[actualBlockNum].BRy;
            vbo[actualBlockNum*18 + 13] = blockList[actualBlockNum].BRy;
            vbo[actualBlockNum*18 + 16] = blockList[actualBlockNum].BRy;

            vbo[actualBlockNum*18 + 0] = blockList[actualBlockNum].ULx;
            vbo[actualBlockNum*18 + 6] = blockList[actualBlockNum].ULx;
            vbo[actualBlockNum*18 + 15] = blockList[actualBlockNum].ULx;

            vbo[actualBlockNum*18 + 3] = blockList[actualBlockNum].BRx;
            vbo[actualBlockNum*18 + 9] = blockList[actualBlockNum].BRx;
            vbo[actualBlockNum*18 + 12] = blockList[actualBlockNum].BRx;

            updateVBOs();
        }
    }
    if(action == GLFW_RELEASE){
        if(key == GLFW_KEY_LEFT_SHIFT){
            BOOL_SHIFT = false;
        }
        if(key == GLFW_KEY_LEFT_CONTROL){
            BOOL_CONTROL = false;
        }

        if(key == GLFW_KEY_W){
            BOOL_W = false;
        }
        if(key == GLFW_KEY_S){
            BOOL_S = false;
        }
        if(key == GLFW_KEY_A){
            BOOL_A = false;
        }
        if(key == GLFW_KEY_D){
            BOOL_D = false;
        }

        if(key == GLFW_KEY_UP){
            BOOL_UP = false;
        }
        if(key == GLFW_KEY_DOWN){
            BOOL_DOWN = false;
        }
        if(key == GLFW_KEY_RIGHT){
            BOOL_RIGHT = false;
        }
        if(key == GLFW_KEY_LEFT){
            BOOL_LEFT = false;
        }
    }
}

void mousefunc(GLFWwindow * window, int button, int x, int y){
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        //std::cout << "bal katt: " << (GLuint)state << std::endl;
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if(state == GLFW_PRESS){
            BOOL_LEFTCLICK = true;
            if(testmode && isTexturing){
                if(isTextureSelector == false){
                    blockList[actualBlockNum].texID = textureSelector->getActiveTex();
                }
            } else {
                if(isTextureSelector == false){
                    //set UpperLeft corner
                    if(BOOL_SHIFT != true && testmode != true){
                        triggerOnce = true;
                    }

                    GLfloat mx = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
                    GLfloat my = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

                    for(int i=0;i<blockNum;i++){
                        if(mx < blockList[i].ULx && mx > blockList[i].BRx){
                            if(my < blockList[i].ULy && my > blockList[i].BRy){
                                blockList[i].isHover = true;
                                if(BOOL_LEFTCLICK == true){
                                    blockList[i].isClicked = true;
                                }
                            }else{
                                blockList[i].isHover = false;
                            }
                        }else{
                            blockList[i].isHover = false;
                        }
                    }
                }
            }
        }else{//GLFW_RELEASE
            BOOL_LEFTCLICK = false;
            if(isTextureSelector == false){
                //set BottomRight corner
                if(BOOL_SHIFT != true && testmode != true){
                    cout << "New block finished!" << endl << "[" << blockList[actualBlockNum].ULx << "," << blockList[actualBlockNum].ULy << "]" << endl << "[" << blockList[actualBlockNum].BRx << "," << blockList[actualBlockNum].BRy << "]" << endl;
                }

                for(int i=0;i<blockNum;i++){
                    blockList[i].isClicked = false;
                }
            }
        }
    }
    if(button == GLFW_MOUSE_BUTTON_MIDDLE){
        //std::cout << "bal katt: " << (GLuint)state << std::endl;
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
        if(state == GLFW_PRESS){
            testmode = !testmode;
        }else{//GLFW_RELEASE
        }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        if(isTextureSelector == false){
            //std::cout << "jobb katt: " << (GLuint)state << std::endl;
            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
            if(state == GLFW_PRESS){
                BOOL_RIGHTCLICK = true;
                if(testmode && isTexturing){
                    textureSelector->setActiveTex(blockList[actualBlockNum].texID);
                } else {
                    isUL = !isUL;
                }
            }else{//GLFW_RELEASE
                BOOL_RIGHTCLICK = false;
            }
        }
    }
}

void scrollfunc(GLFWwindow * window, double x, double y){
    if(y > scrollprev){//scrolling up

    }
    if(y < scrollprev){//scrolling down

    }

    scrollprev = y;
}

GLint prevMX = 0;
GLint prevMY = 0;

void movemouse(GLFWwindow * window, double xPos, double yPos){
    //cout << MX << endl;
    //cout << MY << endl;

    MX = awidth-xPos;
    MY = aheight-yPos;

    if(BOOL_CONTROL && blockNum > 0 && testmode == false){//move blocks with mouse
        GLfloat myRounded = ((GLint)(((GLfloat)(MY) / (GLfloat)(aheight))*200) / 200.0  -  (GLint)(((GLfloat)(prevMY) / (GLfloat)(aheight))*200) / 200.0) *2;
        GLfloat mxRounded = ((GLint)(((GLfloat)(MX) / (GLfloat)(awidth))*200) / 200.0  -  (GLint)(((GLfloat)(prevMX) / (GLfloat)(awidth))*200) / 200.0) *2;

        blockList[actualBlockNum].ULy += myRounded;
        blockList[actualBlockNum].BRy += myRounded;

        vbo[actualBlockNum*18 + 1] = blockList[actualBlockNum].ULy;
        vbo[actualBlockNum*18 + 4] = blockList[actualBlockNum].ULy;
        vbo[actualBlockNum*18 + 10] = blockList[actualBlockNum].ULy;

        vbo[actualBlockNum*18 + 7] = blockList[actualBlockNum].BRy;
        vbo[actualBlockNum*18 + 13] = blockList[actualBlockNum].BRy;
        vbo[actualBlockNum*18 + 16] = blockList[actualBlockNum].BRy;


        blockList[actualBlockNum].ULx += mxRounded;
        blockList[actualBlockNum].BRx += mxRounded;

        vbo[actualBlockNum*18 + 0] = blockList[actualBlockNum].ULx;
        vbo[actualBlockNum*18 + 6] = blockList[actualBlockNum].ULx;
        vbo[actualBlockNum*18 + 15] = blockList[actualBlockNum].ULx;

        vbo[actualBlockNum*18 + 3] = blockList[actualBlockNum].BRx;
        vbo[actualBlockNum*18 + 9] = blockList[actualBlockNum].BRx;
        vbo[actualBlockNum*18 + 12] = blockList[actualBlockNum].BRx;

        updateVBOs();
    }

    prevMX = MX;
    prevMY = MY;

    GLfloat mx = (((GLfloat)(MX)/(GLfloat)(awidth)-0.5)*2.0);
    GLfloat my = (((GLfloat)(MY)/(GLfloat)(aheight)-0.5)*2.0);

    GLint prevActual = 0;
    for(int i=0;i<blockNum;i++){
        if(mx < blockList[i].ULx && mx > blockList[i].BRx){
            if(my < blockList[i].ULy && my > blockList[i].BRy){
                blockList[i].isHover = true;
                //if((fabs(blockList[prevActual].BRx - blockList[prevActual].ULx) * fabs(blockList[prevActual].BRy - blockList[prevActual].ULy)) >
                //   (fabs(blockList[i].BRx - blockList[i].ULx) * fabs(blockList[i].BRy - blockList[i].ULy)) && testmode == true){
                if(testmode == true){
                    prevActual = i;
                    actualBlockNum = i;
                }
                //}
            }else{
                blockList[i].isHover = false;
            }
        }else{
            blockList[i].isHover = false;
        }
    }

    /*szogy -= (MX - awidth/2)/2.0f;
    szogx -= (MY - aheight/2)/2.0f;
    if(szogx > 89){
        szogx = 89;
    }
    if(szogx < -89){
        szogx = -89;
    }*/
}

bool MOUSE_CLICK_LEFT_TITLE = false;
bool MOUSE_RIGHT_CLICK_TITLE = false;
double clickPosX = 0;
double clickPosY = 0;

void mousefuncTitle(GLFWwindow * window, int button, int x, int y){
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        //std::cout << "bal katt: " << (GLuint)state << std::endl;
        int state = glfwGetMouseButton(windowTitle, GLFW_MOUSE_BUTTON_LEFT);
        if(state == GLFW_PRESS){
            MOUSE_CLICK_LEFT_TITLE = true;
            glfwGetCursorPos(windowTitle, &clickPosX, &clickPosY);
            //clickPosX = x;
            //clickPosY = y;
        }else{//GLFW_RELEASE
            MOUSE_CLICK_LEFT_TITLE = false;
        }
    }
    if(button == GLFW_MOUSE_BUTTON_MIDDLE){
        //std::cout << "bal katt: " << (GLuint)state << std::endl;
        int state = glfwGetMouseButton(windowTitle, GLFW_MOUSE_BUTTON_MIDDLE);
        if(state == GLFW_PRESS){

        }else{//GLFW_RELEASE

        }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        //std::cout << "jobb katt: " << (GLuint)state << std::endl;
        int state = glfwGetMouseButton(windowTitle, GLFW_MOUSE_BUTTON_RIGHT);
        if(state == GLFW_PRESS){
            MOUSE_RIGHT_CLICK_TITLE = true;
        }else{//GLFW_RELEASE
            MOUSE_RIGHT_CLICK_TITLE = false;
        }
    }
}

void scrollfuncTitle(GLFWwindow * window, double x, double y){
    if(y > scrollprev){//scrolling up

    }
    if(y < scrollprev){//scrolling down

    }

    scrollprev = y;
}

GLint prevMXTitle = 0;
GLint prevMYTitle = 0;

void movemouseTitle(GLFWwindow * window, double xPos, double yPos){
    //cout << MX << endl;
    //cout << MY << endl;

    MXTitle = xPos;
    MYTitle = yPos;

    if(MOUSE_CLICK_LEFT_TITLE && awidthTitle*0.30 < MXTitle && awidthTitle*0.85 > MXTitle){
        movedX = (floor(MXTitle) - floor(prevMXTitle))*2;
        movedY = (floor(MYTitle) - floor(prevMYTitle))*2;

        prevMXTitle = clickPosX;
        prevMYTitle = clickPosY;
    }else{
        prevMXTitle = MXTitle;
        prevMYTitle = MYTitle;
    }
}

int initGLFW()
{
    // Initialise GLFW
    glewExperimental = true; // Needed for core profile
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    //GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow( 1152, 648, "MAP Generator", NULL, NULL);

    windowTitle = glfwCreateWindow( 1152,  40, "MAP Generator Title Bar", NULL, NULL);

    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(0);

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glfwSwapInterval(0);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwSetWindowSizeCallback(window, reshape);
    glfwSetWindowSizeCallback(windowTitle, reshapeTitle);

    glfwSetKeyCallback(window, keyhandle);

    glfwSetMouseButtonCallback(window, mousefunc);
    glfwSetCursorPosCallback(window, movemouse);
    glfwSetScrollCallback(window, scrollfunc);

    glfwSetMouseButtonCallback(windowTitle, mousefuncTitle);
    glfwSetCursorPosCallback(windowTitle, movemouseTitle);
    glfwSetScrollCallback(windowTitle, scrollfuncTitle);

    return 0;
}

bool isMinimized = false;
bool isMaximized = false;

int main(int argc, char* argv[])
{
    initGLFW();

    init();

    window = glfwGetCurrentContext();

    //glusWindowSwapInterval(1);
    //glfwSwapInterval(1);

    glfwGetWindowPos(::windowTitle, &wtx, &wty);
    GLfloat time = 1.0f / 60.0f;
    // Enter the update render loop. See below state machine for more information.
    do{
        auto t0 = std::chrono::high_resolution_clock::now();

        glfwMakeContextCurrent(windowTitle);

        glBindBuffer(GL_ARRAY_BUFFER, CBOT);
        glBufferSubData(GL_ARRAY_BUFFER, 0, cbot.size() * sizeof(GLubyte), (GLubyte*) cbot.data());

        if(awidthTitle*0.00 < MXTitle && awidthTitle*0.075 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 0;

            if(MOUSE_CLICK_LEFT_TITLE){
                isTextureSelector = false;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
                glfwShowWindow(window);
                MOUSE_CLICK_LEFT_TITLE = false;
                isMinimized = false;
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.075 < MXTitle && awidthTitle*0.15 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 1;

            if(MOUSE_CLICK_LEFT_TITLE){
                isTextureSelector = true;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
                glfwShowWindow(window);
                MOUSE_CLICK_LEFT_TITLE = false;
                isMinimized = false;
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.15 < MXTitle && awidthTitle*0.225 > MXTitle){
            if(MOUSE_CLICK_LEFT_TITLE){

            }
        }
        if(awidthTitle*0.225 < MXTitle && awidthTitle*0.25 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 3;

            if(MOUSE_CLICK_LEFT_TITLE){
                //select menu -
                string idx = to_string(actualMenu);
                saveUI(idx);
                if(actualMenu > 0){
                    actualMenu -= 1;
                }
                idx = to_string(actualMenu);
                loadUI(idx);
                MOUSE_CLICK_LEFT_TITLE = false;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
                glfwShowWindow(window);
                isMinimized = false;
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.25 < MXTitle && awidthTitle*0.275 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 4;

            if(MOUSE_CLICK_LEFT_TITLE){
                //select menu +
                string idx = to_string(actualMenu);
                saveUI(idx);
                if(actualMenu < numMenu){
                    actualMenu += 1;
                }
                idx = to_string(actualMenu);
                loadUI(idx);
                MOUSE_CLICK_LEFT_TITLE = false;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
                glfwShowWindow(window);
                isMinimized = false;
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.275 < MXTitle && awidthTitle*0.30 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 5;

            if(MOUSE_CLICK_LEFT_TITLE){
                if(BOOL_CONTROL == false){
                    //add new menu
                    numMenu++;
                    string idx = to_string(numMenu);
                    createUI(idx);
                    saveUIpro();
                }else{
                    //create new project
                    numMenu = 0;
                    actualMenu = 0;
                    string idx = to_string(numMenu);
                    createUI(idx);
                    //project file needed to work properly

                    saveUIpro();
                    loadUI(idx);
                }
                MOUSE_CLICK_LEFT_TITLE = false;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
                glfwShowWindow(window);
                isMinimized = false;
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.30 < MXTitle && awidthTitle*0.85 > MXTitle){
            if(MOUSE_CLICK_LEFT_TITLE){
                glfwWaitEvents();
                glfwGetWindowPos(::windowTitle, &wtx, &wty);
                glfwWaitEvents();
                glfwSetCursorPos(::windowTitle, clickPosX, clickPosY);
                glfwWaitEvents();
            }
        }
        if(awidthTitle*0.85 < MXTitle && awidthTitle*0.90 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 7;

            if(MOUSE_CLICK_LEFT_TITLE){
                if(isMinimized == false){
                    glfwHideWindow(window);
                    isMinimized = true;
                }else{
                    glfwShowWindow(window);
                    isMinimized = false;
                }
                MOUSE_CLICK_LEFT_TITLE = false;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.90 < MXTitle && awidthTitle*0.95 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 8;

            if(MOUSE_CLICK_LEFT_TITLE){
                GLFWmonitor* primary = glfwGetPrimaryMonitor();

                const GLFWvidmode* mode = glfwGetVideoMode(primary);
                if(isMaximized == false){
                    glfwSetWindowSize(window, mode->width, mode->height - 40);
                    glfwSetWindowSize(windowTitle, mode->width, 40);
                    isMaximized = true;
                }else{
                    glfwSetWindowSize(window, 1152, 648);
                    glfwSetWindowSize(windowTitle, 1152, 40);
                    isMaximized = false;
                }
                MOUSE_CLICK_LEFT_TITLE = false;
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }
        if(awidthTitle*0.95 < MXTitle && awidthTitle*1.00 > MXTitle){
            vector<GLubyte> localCBO;
            localCBO.resize(24);
            GLint IDX = 9;

            if(MOUSE_CLICK_LEFT_TITLE){
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 30;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 30;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 30;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }else{
                for(int i=0; i<6; i++){
                    localCBO[i*4 + 0] = cbot[IDX*24 + i*4 + 0] + 15;
                    localCBO[i*4 + 1] = cbot[IDX*24 + i*4 + 1] + 15;
                    localCBO[i*4 + 2] = cbot[IDX*24 + i*4 + 2] + 15;
                    localCBO[i*4 + 3] = cbot[IDX*24 + i*4 + 3];
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, CBOT);
            glBufferSubData(GL_ARRAY_BUFFER, 0 + IDX*24, (24) * sizeof(GLubyte), (GLubyte*) localCBO.data());
        }

        glfwMakeContextCurrent(window);

        update(time);

        glfwPollEvents();
        glfwSwapBuffers(window);
        glfwMakeContextCurrent(windowTitle);

        updateTitleBar();

        glfwPollEvents();
        glfwSwapBuffers(windowTitle);

        wtx += movedX;
        wty += movedY;
        if(wtx < 0){
            wtx = 0;
        }
        if(wty < 0){
            wty = 0;
        }

        glfwMakeContextCurrent(window);
        glfwSetWindowPos(::window, wtx, wty + 40);

        glfwMakeContextCurrent(windowTitle);
        glfwSetWindowPos(::windowTitle, wtx, wty);
        glfwWaitEvents();

        movedX = 0;
        movedY = 0;

        auto t1 = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
        time = (GLfloat)(diff.count()) / 1000.0f;
    } // Check if the ESC key was pressed or the window was closed
    while( (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwGetKey(windowTitle, GLFW_KEY_ESCAPE ) != GLFW_PRESS) &&
           glfwWindowShouldClose(window) == 0 );

    return 0;
}

