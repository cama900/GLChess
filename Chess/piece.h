/*
#include <GL/gl.h>
#include <GL/glut.h>
#include "glm.h"
these three things must be included before the piece class
*/

class Piece{
    public:
        Piece(float x, float y, float z, float r, float g, float b, float sf, GLMmodel *m);
        Piece();
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
        float sf;
        GLMmodel *model;
        void draw();
};

Piece::Piece(float x, float y, float z, float r, float g, float b, float sf, GLMmodel *m){
    Piece::x = x;
    Piece::y = y;
    Piece::z = z;
    Piece::r = r;
    Piece::g = g;
    Piece::b = b;
    Piece::sf = sf;
    Piece::model = m;
}

Piece::Piece(){
    Piece::x = 0;
    Piece::y = 0;
    Piece::z = 0;
    Piece::r = 0;
    Piece::g = 0;
    Piece::b = 0;
    Piece::sf = 0;
    Piece::model = (GLMmodel*)malloc(sizeof(GLMmodel));
}


void Piece::draw(){
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sf, sf, sf);
    glColor3f(r, g, b);
    glmDraw(model, GLM_SMOOTH | GL_FILL);
    glPopMatrix();
}
