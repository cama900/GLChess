/* created by Cameron Holiman and Douglas griffin
models from http://www.turbosquid.com/FullPreview/Index.cfm/ID/686549
*/

#include <GL/gl.h>
#include <GL/glut.h>
#include "glm.h"
#include <ctime>
#include "piece.h"
#include <iostream>
#include <typeinfo>
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

Piece pawn1;

enum color {BLACK, WHITE, GRAY, BROWN};
color currentColor = BLACK;

GLMmodel *pawn, *rook, *knight, *bishop, *queen, *king;

int displayList = 0;

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
    glRotatef(90, 1, 0, 0); // rotate along the x axis so the peices will be upright
    double sf = .08; // set the scale factor so that the peices will be smaller

    pawn1.draw();
   /* for(int i = 0; i < 8; i++){
        glScalef(sf, sf, sf); // scale using the scale factor
        glmDraw(pawn, GLM_SMOOTH | GL_FILL); //draw the pawn
        glScalef(1/sf, 1/sf, 1/sf);//unscale
        glTranslatef(1, 0, 0);//move to next pawn position
        if(i % 2 != 0){ //change the pawn color
            glColor3f(.2, .12, .08);
        }
        else glColor3f(.7, .6, .35);
    }*/


    //glRotatef(angle,1,1,1);

    //glutSolidTeapot(1);

    //glColor3f(.1,.1,.1);
    //glCallList(displayList);
    //glutSolidTeapot(2);

    glutSwapBuffers();
    dt = clock() - startClock; //find the time
    angle += 360*.1*dt/1000;
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y) {
    if (key=='s')
    {
        if (specular==false)
        {
            specular = true;
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
        }
        else if (specular==true)
        {
            specular = false;
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blankMaterial);
        }
    }

    if (key=='d')
    {
        if (diffuse==false)
        {
            diffuse = true;
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
        }
        else if (diffuse==true)
        {
            diffuse = false;
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blankMaterial);
        }
    }

    if (key=='e')
    {
        if (emissive==false)
        {
            emissive = true;
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, greenEmissiveMaterial);
        }
        else if (emissive==true)
        {
            emissive = false;
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blankMaterial);
        }
    }
}

Piece getPiece(float x, float y, float z, float r, float g, float b, float sf, GLMmodel *m){
    return Piece(x, z, y, r, g, b, sf, m);
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("A basic OpenGL Window");

    init ();
    glutDisplayFunc (display);
    glutIdleFunc (display);
    glutKeyboardFunc (keyboard);
    glutReshapeFunc (reshape);

    pawn = (GLMmodel*)malloc(sizeof(GLMmodel));
    /*knight = (GLMmodel*)malloc(sizeof(GLMmodel));
    king = (GLMmodel*)malloc(sizeof(GLMmodel));
    queen = (GLMmodel*)malloc(sizeof(GLMmodel));
    rook = (GLMmodel*)malloc(sizeof(GLMmodel));
    bishop = (GLMmodel*)malloc(sizeof(GLMmodel));
    knight = (GLMmodel*)malloc(sizeof(GLMmodel));*/

    pawn = glmReadOBJ("\pawn.obj");
    /*knight = glmReadOBJ("\knight.obj");
    king = glmReadOBJ("\king.obj");
    queen = glmReadOBJ("\queen.obj");
    rook = glmReadOBJ("\rook.obj");
    bishop = glmReadOBJ("\bishop.obj");
    knight = glmReadOBJ("\knight.obj");*/
    //std::cout << "type of pawn1: " << typeid(pawn1).name();
    //pawn1 = (Piece*)malloc(sizeof(Piece));
    //(0.0f, 0.0f, 0.0f, .1f, .1f, .1f, pawn)
    pawn1 = getPiece(0.0f, 0.0f, 1.0f, .1f, .1f, .1f, .08f, pawn);


	//displayList=glGenLists(1);
	//glNewList(displayList,GL_COMPILE);
		//glmList(stars, GLM_SMOOTH);
		//glmDraw(stars, GLM_SMOOTH | GL_FILL);
	//glEndList();

    glutMainLoop ();

    return 0;
}
