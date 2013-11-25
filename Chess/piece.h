/*
#include <GL/gl.h>
#include <GL/glut.h>
#include "glm.h"
these three things must be included before the piece class
*/

class Piece{
    public:
        Piece(float x, float y, float z, float r, float g, float b, float sf, float rot, GLMmodel *m);
        Piece();
        float x;
        float y;
        float z;
        float dx;
        float dy;
        float dz;
        float r;
        float g;
        float b;
        float sf;
        float rot;
        GLMmodel *model;
        void draw(double dt);
        void updatePosition(double dt);
        void setY(float n);
        float getY();
        void setNextMove(Piece *p, float x, float y);
        bool hasUpdatedPosition;
    private:
        bool hasNextMove;
        Piece *nextPiece;
        float nextX;
        float nextY;
        double moveDistance;
        double xMoveDistance;
        double yMoveDistance;
        int steps;
        double a;
        void updateDistVar();
        double totalMoveTime;
        double moveTime;
};

Piece::Piece(float x, float y, float z, float r, float g, float b, float sf, float rot, GLMmodel *m){
    Piece::x = dx = x;
    Piece::y = dy = y;
    Piece::z = dz = z;
    Piece::r = r;
    Piece::g = g;
    Piece::b = b;
    Piece::sf = sf;
    Piece::rot = rot;
    Piece::model = m;
}

Piece::Piece(){
    Piece::x = dx = 0;
    Piece::y = dy = 0;
    Piece::z = dz = 0;
    Piece::r = 0;
    Piece::g = 0;
    Piece::b = 0;
    Piece::sf = 0;
    Piece::rot = rot;
    Piece::model = (GLMmodel*)malloc(sizeof(GLMmodel));
}

void Piece::setY(float n){
    y = n;
}

float Piece::getY(){
    return y;
}

void Piece::draw(double dt){
    if(y != dy || x != dx){
        if(hasUpdatedPosition == false){
            updateDistVar();
            hasUpdatedPosition = true;
        }
        updatePosition(dt);
    }
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sf, sf, sf);
    glRotatef(rot, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    glColor3f(r, g, b);
    glmDraw(model, GLM_SMOOTH | GL_FILL);
    glPopMatrix();
}


void Piece::updateDistVar(){
    moveDistance = sqrt(pow((dx-x),2)+pow((dy-y),2)); // total length of major axis (path of movement)
    xMoveDistance = (dx - x);
    yMoveDistance = (dy - y);
    totalMoveTime = moveDistance*1000;
    moveTime = 0;
}

void Piece::updatePosition(double dt) {
    /* If moving straight forward use y distance, otherwise use x distance*/
    /*
        steps = (dy - y) * 20; // how many renderings to make the move
    }
    else {
        steps = (dx - x) * 20; // how many renderings to make the move
    }*/

    a = moveDistance/2;
    if (x != dx) {
            x += (xMoveDistance*dt/totalMoveTime);
    }
    if (y != dy) {
            y += (yMoveDistance*dt/totalMoveTime);
    }

    //z = z + (sqrt(4*(1-((x * x)/(a * a)))));

    moveTime += dt;
    if(moveTime >= totalMoveTime){ // if the move is over
        x = dx;
        y = dy;
        z = dz;

        moveDistance = 0;
        hasUpdatedPosition = false;
    }

    std::cout << "x = " << x << " y = " << y << " z = " << z << std::endl;
}

void Piece::setNextMove(Piece *p, float x, float y){
    hasNextMove = true;
    nextPiece = p;
    nextX = x;
    nextY = y;
}

