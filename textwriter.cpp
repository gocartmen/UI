#include "textwriter.h"

void TextWriter::loadData(ifstream &abcData, struct keys &data/*, ofstream &abcDataSave, bool isModify*/)
{
    abcData.read(reinterpret_cast<char*>(&data.x1), sizeof(data.x1));
    abcData.read(reinterpret_cast<char*>(&data.y1), sizeof(data.y1));
    abcData.read(reinterpret_cast<char*>(&data.x2), sizeof(data.x2));
    abcData.read(reinterpret_cast<char*>(&data.y2), sizeof(data.y2));

    /*if(isModify){
        data.y2 = -1;
    }
    abcDataSave.write(reinterpret_cast<const char*>(&data.x1), sizeof(data.x1));
    abcDataSave.write(reinterpret_cast<const char*>(&data.y1), sizeof(data.y1));
    abcDataSave.write(reinterpret_cast<const char*>(&data.x2), sizeof(data.x2));
    abcDataSave.write(reinterpret_cast<const char*>(&data.y2), sizeof(data.y2));*/
}

TextWriter::TextWriter()
{
    GLuint blockNum;

    ifstream abcData;
    stringstream abcFileName;
    abcFileName.str("");
    abcFileName << "savedABCfixed.bin";
    abcData.open(abcFileName.str().c_str(), ios::in | ios::binary);

    abcData.read((char*)(&blockNum), sizeof(blockNum));

    /*ofstream abcDataSave;
    stringstream abcFileNameSave;
    abcFileNameSave.str("");
    abcFileNameSave << "savedABCfixed.bin";
    abcDataSave.open(abcFileNameSave.str().c_str(), ios::out | ios::binary);

    abcDataSave.write((const char*)(&blockNum), sizeof(blockNum));*/


    struct keys data;
    loadData(abcData, data);
    translator['!'] = data;

    loadData(abcData, data);
    translator['"'] = data;

    loadData(abcData, data);
    translator['#'] = data;

    loadData(abcData, data);
    translator['$'] = data;

    loadData(abcData, data);
    translator['%'] = data;

    loadData(abcData, data);
    translator['&'] = data;

    loadData(abcData, data);
    translator['\''] = data;

    loadData(abcData, data);
    translator['('] = data;

    loadData(abcData, data);
    translator[')'] = data;

    loadData(abcData, data);
    translator['*'] = data;

    loadData(abcData, data);
    translator['+'] = data;

    loadData(abcData, data);
    translator[','] = data;

    loadData(abcData, data);
    translator['-'] = data;

    loadData(abcData, data);
    translator['.'] = data;

    loadData(abcData, data);
    translator['/'] = data;

    loadData(abcData, data);
    translator['0'] = data;

    loadData(abcData, data);
    translator['1'] = data;

    loadData(abcData, data);
    translator['2'] = data;

    loadData(abcData, data);
    translator['3'] = data;

    loadData(abcData, data);
    translator['4'] = data;

    loadData(abcData, data);
    translator['5'] = data;

    loadData(abcData, data);
    translator['6'] = data;

    loadData(abcData, data);
    translator['7'] = data;

    loadData(abcData, data);
    translator['8'] = data;

    loadData(abcData, data);
    translator['9'] = data;

    loadData(abcData, data);
    translator[':'] = data;

    loadData(abcData, data);
    translator[';'] = data;

    loadData(abcData, data);
    translator['<'] = data;

    loadData(abcData, data);
    translator['='] = data;

    loadData(abcData, data);
    translator['>'] = data;

    loadData(abcData, data);
    translator['?'] = data;

    loadData(abcData, data);
    translator['@'] = data;

    loadData(abcData, data);
    translator['A'] = data;

    loadData(abcData, data);
    translator['B'] = data;

    loadData(abcData, data);
    translator['C'] = data;

    loadData(abcData, data);
    translator['D'] = data;

    loadData(abcData, data);
    translator['E'] = data;

    loadData(abcData, data);
    translator['F'] = data;

    loadData(abcData, data);
    translator['G'] = data;

    loadData(abcData, data);
    translator['H'] = data;

    loadData(abcData, data);
    translator['I'] = data;

    loadData(abcData, data);
    translator['J'] = data;

    loadData(abcData, data);
    translator['K'] = data;

    loadData(abcData, data);
    translator['L'] = data;

    loadData(abcData, data);
    translator['M'] = data;

    loadData(abcData, data);
    translator['N'] = data;

    loadData(abcData, data);
    translator['O'] = data;

    loadData(abcData, data);
    translator['P'] = data;

    loadData(abcData, data);
    translator['Q'] = data;

    loadData(abcData, data);
    translator['R'] = data;

    loadData(abcData, data);
    translator['S'] = data;

    loadData(abcData, data);
    translator['T'] = data;

    loadData(abcData, data);
    translator['U'] = data;

    loadData(abcData, data);
    translator['V'] = data;

    loadData(abcData, data);
    translator['W'] = data;

    loadData(abcData, data);
    translator['X'] = data;

    loadData(abcData, data);
    translator['Y'] = data;

    loadData(abcData, data);
    translator['Z'] = data;

    loadData(abcData, data);
    translator['['] = data;

    loadData(abcData, data);
    translator['\\'] = data;

    loadData(abcData, data);
    translator[']'] = data;

    loadData(abcData, data);
    translator['^'] = data;

    loadData(abcData, data);
    translator['_'] = data;

    loadData(abcData, data);
    translator['`'] = data;

    loadData(abcData, data);
    translator['a'] = data;

    loadData(abcData, data);
    translator['b'] = data;

    loadData(abcData, data);
    translator['c'] = data;

    loadData(abcData, data);
    translator['d'] = data;

    loadData(abcData, data);
    translator['e'] = data;

    loadData(abcData, data);
    translator['f'] = data;

    loadData(abcData, data);
    translator['g'] = data;

    loadData(abcData, data);
    translator['h'] = data;

    loadData(abcData, data);
    translator['i'] = data;

    loadData(abcData, data);
    translator['j'] = data;

    loadData(abcData, data);
    translator['k'] = data;

    loadData(abcData, data);
    translator['l'] = data;

    loadData(abcData, data);
    translator['m'] = data;

    loadData(abcData, data);
    translator['n'] = data;

    loadData(abcData, data);
    translator['o'] = data;

    loadData(abcData, data);
    translator['p'] = data;

    loadData(abcData, data);
    translator['q'] = data;

    loadData(abcData, data);
    translator['r'] = data;

    loadData(abcData, data);
    translator['s'] = data;

    loadData(abcData, data);
    translator['t'] = data;

    loadData(abcData, data);
    translator['u'] = data;

    loadData(abcData, data);
    translator['v'] = data;

    loadData(abcData, data);
    translator['w'] = data;

    loadData(abcData, data);
    translator['x'] = data;

    loadData(abcData, data);//true
    translator['y'] = data;

    loadData(abcData, data);
    translator['z'] = data;

    loadData(abcData, data);
    translator['{'] = data;

    loadData(abcData, data);
    translator['|'] = data;

    loadData(abcData, data);
    translator['}'] = data;

    loadData(abcData, data);
    translator['~'] = data;

    data.x1 = -0.94;
    data.y1 = -1.0;
    data.x2 = -1.0;
    data.y2 = -1.0;
    translator[' '] = data;

    abcData.close();

    //abcDataSave.close();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 1024*1024*3 * sizeof(GLfloat), (GLfloat*) 0, GL_STREAM_DRAW);

    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, 1024*1024*4 * sizeof(GLubyte), (GLubyte*) 0, GL_STREAM_DRAW);

    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, 1024*1024*2 * sizeof(GLfloat), (GLfloat*) 0, GL_STREAM_DRAW);
}

void TextWriter::addText(string newText, GLfloat x, GLfloat y)
{
    text.push_back(newText);
    struct pos newPos;
    newPos.x = x;
    newPos.y = y;
    textPos.push_back(newPos);
    
    GLfloat textWidth = 0.0;
    GLfloat textHeight = 0.0;
    for(int i=0;i<newText.length();i++){
        if(newText[i] == 'g' || newText[i] == 'j' || newText[i] == 'p' || newText[i] == 'q' || newText[i] == 'y'){
            textHeight = -0.006;
        }else{
            textHeight = 0.0;
        }

        vbo.push_back(x + textWidth);
        vbo.push_back(y + textHeight);
        vbo.push_back(1.0);

        vbo.push_back(x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
        vbo.push_back(y + textHeight);
        vbo.push_back(1.0);

        vbo.push_back(x + textWidth);
        vbo.push_back(y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
        vbo.push_back(1.0);


        vbo.push_back(x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
        vbo.push_back(y + textHeight);
        vbo.push_back(1.0);

        vbo.push_back(x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
        vbo.push_back(y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
        vbo.push_back(1.0);

        vbo.push_back(x + textWidth);
        vbo.push_back(y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
        vbo.push_back(1.0);

        textWidth += (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0;

        for(int j=0;j<6;j++){
            cbo.push_back(255);
            cbo.push_back(255);
            cbo.push_back(255);
            cbo.push_back(255);
        }


        cout << "x1: " << (translator[newText[i]].x1+1.0)/2.0*255;
        if((translator[newText[i]].x1+1.0)/2.0*255 > 255 || (translator[newText[i]].x1+1.0)/2.0*255 < 0){
            cout << "   ERROR!!" << endl;
        }else{
            cout << endl;
        }
        cout << "x2: " << (translator[newText[i]].x2+1.0)/2.0*255;
        if((translator[newText[i]].x2+1.0)/2.0*255 > 255 || (translator[newText[i]].x2+1.0)/2.0*255 < 0){
            cout << "   ERROR!!" << endl;
        }else{
            cout << endl;
        }
        cout << "y1: " << (translator[newText[i]].y1+1.0)/2.0*255;
        if((translator[newText[i]].y1+1.0)/2.0*255 > 255 || (translator[newText[i]].y1+1.0)/2.0*255 < 0){
            cout << "   ERROR!!" << endl;
        }else{
            cout << endl;
        }
        cout << "y2: " << (translator[newText[i]].y2+1.0)/2.0*255;
        if((translator[newText[i]].y2+1.0)/2.0*255 > 255 || (translator[newText[i]].y2+1.0)/2.0*255 < 0){
            cout << "   ERROR!!" << endl;
        }else{
            cout << endl;
        }

        tbo.push_back((255-(translator[newText[i]].x1+1.0)/2.0*255)/255);
        tbo.push_back(((translator[newText[i]].y2+1.0)/2.0*255)/255);

        tbo.push_back((255-(translator[newText[i]].x2+1.0)/2.0*255)/255);
        tbo.push_back(((translator[newText[i]].y2+1.0)/2.0*255)/255);

        tbo.push_back((255-(translator[newText[i]].x1+1.0)/2.0*255)/255);
        tbo.push_back(((translator[newText[i]].y1+1.0)/2.0*255)/255);


        tbo.push_back((255-(translator[newText[i]].x2+1.0)/2.0*255)/255);
        tbo.push_back(((translator[newText[i]].y2+1.0)/2.0*255)/255);

        tbo.push_back((255-(translator[newText[i]].x2+1.0)/2.0*255)/255);
        tbo.push_back(((translator[newText[i]].y1+1.0)/2.0*255)/255);

        tbo.push_back((255-(translator[newText[i]].x1+1.0)/2.0*255)/255);
        tbo.push_back(((translator[newText[i]].y1+1.0)/2.0*255)/255);

        /*tbo.push_back(0);
        tbo.push_back(255);

        tbo.push_back(255);
        tbo.push_back(255);

        tbo.push_back(0);
        tbo.push_back(0);


        tbo.push_back(255);
        tbo.push_back(255);

        tbo.push_back(255);
        tbo.push_back(0);

        tbo.push_back(0);
        tbo.push_back(0);*/
    }


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vbo.size() * sizeof(GLfloat), (GLfloat*) vbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cbo.size() * sizeof(GLubyte), (GLubyte*) cbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tbo.size() * sizeof(GLfloat), (GLfloat*) tbo.data());
}

void TextWriter::updateText(GLuint szam, string newText, GLubyte r, GLubyte g, GLubyte b)
{
    float x = textPos[szam].x;
    float y = textPos[szam].y;

    int offset = 0;
    for(int i=0;i<szam;i++){
        offset += text[i].length()*6;
    }

    if(newText.length() > text[szam].length()){
        int lenDiff = newText.length() - text[szam].length();
        for(int i=0;i<lenDiff;i++){
            for(int j=0;j<6;j++){
                vbo.push_back(0);
                vbo.push_back(0);
                vbo.push_back(0);

                cbo.push_back(0);
                cbo.push_back(0);
                cbo.push_back(0);
                cbo.push_back(0);

                tbo.push_back(0);
                tbo.push_back(0);
            }
        }

        //copy to new location
        for(int i=vbo.size()-1; i>offset*3+lenDiff*18; i--){
            vbo[i] = vbo[i-lenDiff*18];
        }
        for(int i=cbo.size()-1; i>offset*4+lenDiff*24; i--){
            cbo[i] = cbo[i-lenDiff*24];
        }
        for(int i=tbo.size()-1; i>offset*2+lenDiff*12; i--){
            tbo[i] = tbo[i-lenDiff*12];
        }
        //--------------------

        GLfloat textWidth = 0.0;
        GLfloat textHeight = 0.0;
        for(int i=0;i<newText.length();i++){
            if(newText[i] == 'g' || newText[i] == 'j' || newText[i] == 'p' || newText[i] == 'q' || newText[i] == 'y'){
                textHeight = -0.006;
            }else{
                textHeight = 0.0;
            }

            vbo[offset*3 + i*18 + 0] = (x + textWidth);
            vbo[offset*3 + i*18 + 1] = (y + textHeight);
            vbo[offset*3 + i*18 + 2] = (1.0);

            vbo[offset*3 + i*18 + 3] = (x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
            vbo[offset*3 + i*18 + 4] = (y + textHeight);
            vbo[offset*3 + i*18 + 5] = (1.0);

            vbo[offset*3 + i*18 + 6] = (x + textWidth);
            vbo[offset*3 + i*18 + 7] = (y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
            vbo[offset*3 + i*18 + 8] = (1.0);


            vbo[offset*3 + i*18 + 9] = (x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
            vbo[offset*3 + i*18 + 10] = (y + textHeight);
            vbo[offset*3 + i*18 + 11] = (1.0);

            vbo[offset*3 + i*18 + 12] = (x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
            vbo[offset*3 + i*18 + 13] = (y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
            vbo[offset*3 + i*18 + 14] = (1.0);

            vbo[offset*3 + i*18 + 15] = (x + textWidth);
            vbo[offset*3 + i*18 + 16] = (y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
            vbo[offset*3 + i*18 + 17] = (1.0);

            textWidth += (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0;

            for(int j=0;j<6;j++){
                cbo[offset*4 + i*24 + j*4 + 0] = (255);
                cbo[offset*4 + i*24 + j*4 + 1] = (255);
                cbo[offset*4 + i*24 + j*4 + 2] = (255);
                cbo[offset*4 + i*24 + j*4 + 3] = (255);
            }


            /*cout << "x1: " << (translator[newText[i]].x1+1.0)/2.0*255;
            if((translator[newText[i]].x1+1.0)/2.0*255 > 255 || (translator[newText[i]].x1+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }
            cout << "x2: " << (translator[newText[i]].x2+1.0)/2.0*255;
            if((translator[newText[i]].x2+1.0)/2.0*255 > 255 || (translator[newText[i]].x2+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }
            cout << "y1: " << (translator[newText[i]].y1+1.0)/2.0*255;
            if((translator[newText[i]].y1+1.0)/2.0*255 > 255 || (translator[newText[i]].y1+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }
            cout << "y2: " << (translator[newText[i]].y2+1.0)/2.0*255;
            if((translator[newText[i]].y2+1.0)/2.0*255 > 255 || (translator[newText[i]].y2+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }*/

            tbo[offset*2 + i*12 + 0] = (255-(translator[newText[i]].x1+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 1] = ((translator[newText[i]].y2+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 2] = (255-(translator[newText[i]].x2+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 3] = ((translator[newText[i]].y2+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 4] = (255-(translator[newText[i]].x1+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 5] = ((translator[newText[i]].y1+1.0)/2.0*255)/255;


            tbo[offset*2 + i*12 + 6] = (255-(translator[newText[i]].x2+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 7] = ((translator[newText[i]].y2+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 8] = (255-(translator[newText[i]].x2+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 9] = ((translator[newText[i]].y1+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 10] = (255-(translator[newText[i]].x1+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 11] = ((translator[newText[i]].y1+1.0)/2.0*255)/255;
        }
    }else{
        if(newText.length() < text[szam].length()){//erasing elements from vbo, cbo and tbo to correct memory overflow
            int lenDiff = text[szam].length() - newText.length();

            //copy to new location
            for(int i=offset*3+text[szam].length()*18-lenDiff*18; i<vbo.size(); i++){
                vbo[i] = vbo[i+lenDiff*18];
            }
            for(int i=offset*4+text[szam].length()*24-lenDiff*24; i<cbo.size(); i++){
                cbo[i] = cbo[i+lenDiff*24];
            }
            for(int i=offset*2+text[szam].length()*12-lenDiff*12; i<tbo.size(); i++){
                tbo[i] = tbo[i+lenDiff*12];
            }
            //--------------------

            vbo.erase(vbo.end()-lenDiff*18, vbo.end());
            cbo.erase(cbo.end()-lenDiff*24, cbo.end());
            tbo.erase(tbo.end()-lenDiff*12, tbo.end());
        }

        GLfloat textWidth = 0.0;
        GLfloat textHeight = 0.0;
        for(int i=0;i<newText.length();i++){
            if(newText[i] == 'g' || newText[i] == 'j' || newText[i] == 'p' || newText[i] == 'q' || newText[i] == 'y'){
                textHeight = -0.006;
            }else{
                textHeight = 0.0;
            }

            vbo[offset*3 + i*18 + 0] = (x + textWidth);
            vbo[offset*3 + i*18 + 1] = (y + textHeight);
            vbo[offset*3 + i*18 + 2] = (1.0);

            vbo[offset*3 + i*18 + 3] = (x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
            vbo[offset*3 + i*18 + 4] = (y + textHeight);
            vbo[offset*3 + i*18 + 5] = (1.0);

            vbo[offset*3 + i*18 + 6] = (x + textWidth);
            vbo[offset*3 + i*18 + 7] = (y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
            vbo[offset*3 + i*18 + 8] = (1.0);


            vbo[offset*3 + i*18 + 9] = (x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
            vbo[offset*3 + i*18 + 10] = (y + textHeight);
            vbo[offset*3 + i*18 + 11] = (1.0);

            vbo[offset*3 + i*18 + 12] = (x + textWidth + (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0);
            vbo[offset*3 + i*18 + 13] = (y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
            vbo[offset*3 + i*18 + 14] = (1.0);

            vbo[offset*3 + i*18 + 15] = (x + textWidth);
            vbo[offset*3 + i*18 + 16] = (y + textHeight + (translator[newText[i]].y1 - translator[newText[i]].y2)/5.0);
            vbo[offset*3 + i*18 + 17] = (1.0);

            textWidth += (translator[newText[i]].x2 - translator[newText[i]].x1)/5.0;

            for(int j=0;j<6;j++){
                cbo[offset*4 + i*24 + j*4 + 0] = (255);
                cbo[offset*4 + i*24 + j*4 + 1] = (255);
                cbo[offset*4 + i*24 + j*4 + 2] = (255);
                cbo[offset*4 + i*24 + j*4 + 3] = (255);
            }


            /*cout << "x1: " << (translator[newText[i]].x1+1.0)/2.0*255;
            if((translator[newText[i]].x1+1.0)/2.0*255 > 255 || (translator[newText[i]].x1+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }
            cout << "x2: " << (translator[newText[i]].x2+1.0)/2.0*255;
            if((translator[newText[i]].x2+1.0)/2.0*255 > 255 || (translator[newText[i]].x2+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }
            cout << "y1: " << (translator[newText[i]].y1+1.0)/2.0*255;
            if((translator[newText[i]].y1+1.0)/2.0*255 > 255 || (translator[newText[i]].y1+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }
            cout << "y2: " << (translator[newText[i]].y2+1.0)/2.0*255;
            if((translator[newText[i]].y2+1.0)/2.0*255 > 255 || (translator[newText[i]].y2+1.0)/2.0*255 < 0){
                cout << "   ERROR!!" << endl;
            }else{
                cout << endl;
            }*/

            tbo[offset*2 + i*12 + 0] = (255-(translator[newText[i]].x1+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 1] = ((translator[newText[i]].y2+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 2] = (255-(translator[newText[i]].x2+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 3] = ((translator[newText[i]].y2+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 4] = (255-(translator[newText[i]].x1+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 5] = ((translator[newText[i]].y1+1.0)/2.0*255)/255;


            tbo[offset*2 + i*12 + 6] = (255-(translator[newText[i]].x2+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 7] = ((translator[newText[i]].y2+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 8] = (255-(translator[newText[i]].x2+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 9] = ((translator[newText[i]].y1+1.0)/2.0*255)/255;

            tbo[offset*2 + i*12 + 10] = (255-(translator[newText[i]].x1+1.0)/2.0*255)/255;
            tbo[offset*2 + i*12 + 11] = ((translator[newText[i]].y1+1.0)/2.0*255)/255;
        }
    }

    text[szam] = newText;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vbo.size() * sizeof(GLfloat), (GLfloat*) vbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cbo.size() * sizeof(GLubyte), (GLubyte*) cbo.data());

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tbo.size() * sizeof(GLfloat), (GLfloat*) tbo.data());
}

void TextWriter::drawText(GLuint program, GLuint texture)
{
    glUseProgram(program);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(glGetAttribLocation(program,"a_vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program,"a_vertex"));

    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glVertexAttribPointer(glGetAttribLocation(program,"a_color"), 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program,"a_color"));

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glVertexAttribPointer(glGetAttribLocation(program,"a_tex0"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program,"a_tex0"));


    glUniform1i(glGetUniformLocation(program,"isTextured"), 1);
    glBindTexture(GL_TEXTURE_2D, texture);


    glDrawArrays(GL_TRIANGLES, 0, vbo.size()/3);


    glDisableVertexAttribArray(glGetAttribLocation(program,"a_vertex"));
    glDisableVertexAttribArray(glGetAttribLocation(program,"a_color"));
    glDisableVertexAttribArray(glGetAttribLocation(program,"a_tex0"));
}
