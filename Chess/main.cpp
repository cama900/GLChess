/* created by Cameron Holiman and Douglas griffin
models from http://www.turbosquid.com/FullPreview/Index.cfm/ID/686549
*/

#include <GL/gl.h>
#include <GL/glut.h>
#include "glm.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <typeinfo>
#include "piece.h"
//#include "glm.h"

GLfloat angle = 0.0;

GLfloat redDiffuseMaterial[] = {.1, .1, .1}; //set the material to red
GLfloat whiteSpecularMaterial[] = {1, 1, 1}; //set the material to white
GLfloat blackSpecularMaterial[] = {.2, .2, .2}; //set the material to white
GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0}; //set the material to green
GLfloat whiteSpecularLight[] = {1, 1, 1}; //set the light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the light ambient to black
GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the diffuse light to white
GLfloat blankMaterial[] = {0.0, 0.0, 0.0}; //set the diffuse light to white
GLfloat mShininess[] = {128}; //set the shininess of the material

bool diffuse = false;
bool emissive = false;
bool specular = false;

unsigned int startClock;
unsigned int dt;
unsigned int pieceClock;

Piece *whitePieces;
Piece *blackPieces;
GLuint pieceList;

float br, bg, bb, wr, wg, wb;

enum color {BLACK, WHITE, GRAY, BROWN};
color currentColor = BLACK;

GLMmodel *pawn, *rook, *knight, *bishop, *queen, *king;

int displayList = 0;


static int getDt(){
    return dt;
}

void init (void) {
    glEnable (GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

void light (void) {
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
}

void flipColor(){ // changes the black to white and white to black
    if(currentColor == BLACK){
        currentColor = WHITE;
        glColor3f(.95,.95,.95);
    }
    else if(currentColor == WHITE){
        currentColor = BLACK;
        glColor3f(.05,.05,.05);
    }
}

void drawBoard(){
    for(int i = 0; i < 8; i++){ //draws the checker board
        for(int j = 0; j < 8; j++){
            glutSolidCube(1);
            glTranslatef(1, 0, 0);
            flipColor();
        }
        glTranslatef(-8, 1, 0);
        flipColor();
    }
    double sf = 10; //new scale factor

    glTranslatef(3.5, 0, 0); //move to the middle and top (already at the top because of the checker drawing)
    glColor3f(.3, .21, .13); //brown


    glScalef(sf, 1, 1); //scale on x
    glutSolidCube(1);//create upper border
    glTranslatef(0, -9, 0); //move to the bottom
    glutSolidCube(1); //create bottom border
    glScalef(1/sf, 1, 1); //unscale

    glTranslatef(-4.5, 4.5, 0); //move to the side

    sf = 8; //set the scale factor
    glScalef(1, sf, 1);//scale
    glutSolidCube(1);//draw the side
    glTranslatef(9, 0, 0); //move to other side
    glutSolidCube(1); //draw that side
    glScalef(1, 1/sf, 1);//unscale

    glTranslatef(-4.5, 0, 0); //move to the middle
}


void display (void) {
    startClock = clock(); // starts the millisecond clock

    glClearColor (0.30,0.30,0.30,1.0); //set the clear color
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the screen
    glLoadIdentity(); //seset the atrix to the identity matrix

    light(); // create the lights and materials

    glColor3f(.05,.05,.05); //set the color to black (near black)
    glTranslatef(0,0,-3); //translate out 3 so things arent drawn on the camera
    glRotatef(-50, 1, 0, 0); // rotate 50 dgrees along the x axis
    glTranslatef(0 , 2, 0); // move the location 2 places down

    glScalef(.5, .5, .5); // scale to 50% in all angles so the board will fit on the screen

    glRotatef(angle,0, 0, 1); //rotate along z axis
    glTranslatef(-3.5, -3, -4); //move out to first board square position

    drawBoard(); // draw the board and border

    glColor3f(.65, .5, .25); //light peice color
    glColor3f(.2, .12, .08); // dark peice color

    glTranslatef(-3.5, -3.5, .5); //move  to the first location for a pawn
    //glRotatef(90, 1, 0, 0); // rotate along the x axis so the peices will be upright

    //glCallList(pieceList);
    for(int i = 0; i < 16; i++){
        blackPieces[i].draw(dt);
        whitePieces[i].draw(dt);
    }

    glutSwapBuffers();
    dt = clock() - startClock; //find the time
    angle += 360*.05*dt/1000;
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y) {
}

Piece getPiece(float x, float y, float z, float r, float g, float b, float sf, GLMmodel *m){
    return Piece(x, y, z, r, g, b, sf, 0.0f, m);
}

Piece getPiece(float x, float y, float z, float r, float g, float b, float sf, float rot, GLMmodel *m){
    return Piece(x, y, z, r, g, b, sf, rot, m);
}

void initPieces(){
    pawn = (GLMmodel*)malloc(sizeof(GLMmodel)); //allocating memory for the piece objects
    knight = (GLMmodel*)malloc(sizeof(GLMmodel));
    king = (GLMmodel*)malloc(sizeof(GLMmodel));
    queen = (GLMmodel*)malloc(sizeof(GLMmodel));
    rook = (GLMmodel*)malloc(sizeof(GLMmodel));
    bishop = (GLMmodel*)malloc(sizeof(GLMmodel));
    knight = (GLMmodel*)malloc(sizeof(GLMmodel));

    pawn = glmReadOBJ("Pawn.obj"); //reading in the obj files for each piece
    knight = glmReadOBJ("Knight.obj");
    king = glmReadOBJ("King.obj");
    queen = glmReadOBJ("Queen.obj");
    rook = glmReadOBJ("Rook.obj");
    bishop = glmReadOBJ("Bishop.obj");
    knight = glmReadOBJ("Knight.obj");

    whitePieces = (Piece*)malloc(sizeof(Piece)*16);
    blackPieces = (Piece*)malloc(sizeof(Piece)*16);


    br = bg = bb = .1f; //setting the colors for the black and white pieces
    wr = wg = wb = .8f;

    //Creating the black pieces
    blackPieces[0] = getPiece(0.0f, 7.0f, 0.0f, br, bg, bb, .06f, rook);
    blackPieces[7 - 0] = getPiece(7.0f - 0.0f, 7.0f, 0.0f, br, bg, bb, .06f, rook);
    blackPieces[1] = getPiece(1.0f, 7.0f, 0.0f, br, bg, bb, .06f, 90.0f, knight);
    blackPieces[7 - 1] = getPiece(7.0f - 1.0f, 7.0f, 0.0f, br, bg, bb, .06f, 90.0f, knight);
    blackPieces[2] = getPiece(2.0f, 7.0f, 0.0f, br, bg, bb, .06f, 90.0f, bishop);
    blackPieces[7 - 2] = getPiece(7.0f - 2.0f, 7.0f, 0.0f, br, bg, bb, .06f, 90.0f, bishop);
    blackPieces[3] = getPiece(3.0f, 7.0f, 0.0f, br, bg, bb, .06f, queen);
    blackPieces[7 - 3] = getPiece(7.0f - 3.0f, 7.0f, 0.0f, br, bg, bb, .06f, king);
    for(int i = 0; i < 8; i++){
        blackPieces[i+8] = getPiece((float)i + 0.0f, 6.0f, 0.0f, .1f, .1f, .1f, .06f, pawn);
    }

    //Creating the white pieces
    whitePieces[0] = getPiece(0.0f, 0.0f, 0.0f, wr, wg, wb, .06f, rook);
    whitePieces[7 - 0] = getPiece(7.0f - 0.0f, 0.0f, 0.0f, wr, wg, wb, .06f, rook);
    whitePieces[1] = getPiece(1.0f, 0.0f, 0.0f, wr, wg, wb, .06f, -90.0f, knight);
    whitePieces[7 - 1] = getPiece(7.0f - 1.0f, 0.0f, 0.0f, wr, wg, wb, .06f, -90.0f, knight);
    whitePieces[2] = getPiece(2.0f, 0.0f, 0.0f, wr, wg, wb, .06f, -90.0f,  bishop);
    whitePieces[7 - 2] = getPiece(7.0f - 2.0f, 0.0f, 0.0f, wr, wg, wb, .06f, -90.0f, bishop);
    whitePieces[3] = getPiece(3.0f, 0.0f, 0.0f, wr, wg, wb, .06f, queen);
    whitePieces[7 - 3] = getPiece(7.0f - 3.0f, 0.0f, 0.0f, wr, wg, wb, .06f, king);
    for(int i = 0; i < 8; i++){
        whitePieces[i+8] = getPiece((float)i + 0.0f, 1.0f, 0.0f, wr, wg, wb, .06f, pawn);
    }

}


int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (900, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("A basic OpenGL Window");

    init ();

    glutDisplayFunc (display);
    glutIdleFunc (display);
    glutKeyboardFunc (keyboard);
    glutReshapeFunc (reshape);


    initPieces();

    whitePieces[12].dy = 3.0f;
    //pawn1 = getPiece(0.0f, 0.0f, 1.0f, .1f, .1f, .1f, .08f, pawn);


	//displayList=glGenLists(1);
	//glNewList(displayList,GL_COMPILE);
		//glmList(stars, GLM_SMOOTH);
		//glmDraw(stars, GLM_SMOOTH | GL_FILL);
	//glEndList();

    glutMainLoop ();

    return 0;
}
