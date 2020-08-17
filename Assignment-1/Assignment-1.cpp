//  ========================================================================
//  COSC363: Computer Graphics (2020); CSSE  University of Canterbury.
//
//  FILE NAME: Assignment-1.cpp
//  See Lab03.pdf for details.
//  ========================================================================

#include <iostream>
#include <cmath>
#include <fstream>
#include <climits>
#include <GL/glut.h>
#include "loadTGA.h"
using namespace std;
#define GL_CLAMP_TO_EDGE 0x812F

GLuint txId[9];   //Texture ids
float angle=0, look_x, look_z=-1., eye_x, eye_z, coll_eye_x = eye_x, coll_eye_z = eye_z;  //Camera parameters
int theta = 15;
int forwards = 0;
int cannonBall_x = 0;
float x1 = 0;


float *x, *y, *z;       //vertex coordinate arrays
int *t1, *t2, *t3;      //triangles
int nvrt, ntri;         //total number of vertices and triangles

const int N = 63;  // Total number of vertices on the base curve

float vx[N] = { 0.0, 12.0, 12, 8, 7.5, 6.7, 5, 5.3, 5, 4.7, 4.5, 4.0, 3.5, 3.3, 2.5, 2.0, 2.0,
					2.0, 2.3, 2.8, 3.7, 3.7, 3.7, 2.8, 2.3, 2.1, 2.3, 3, 3, 5, 7, 8, 9, 10, 11, 11.5, 12, 12.5, 13,
					13.3, 13.5, 13.6, 13.8, 14.0, 14.2, 14.4, 13.5, 13.0, 12.8, 12.5, 12.3, 12.0, 11.6, 11.3, 11.0, 10.6, 10.3, 9.5, 8.5, 7, 6, 5, 0 };
float vy[N] = { 0.0, 0.0, 0.5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
					19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
				    39, 40, 41, 42, 43, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27 };
float vz[N] = { 0 };

//-------------------------------------------------------------------
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

//----------------------------------------------------------------------------
void myTimer(int value)
{
	
    if (theta >= 15){
        forwards = 0;
    }
    if (theta <= -15){
        forwards = 1;
    }
    if (forwards == 0){
        theta--;
    }
    if (forwards == 1){
        theta++;
    }

    glutPostRedisplay();
    glutTimerFunc(25, myTimer, 0);   //(time in ms, what to call, counter value)
}

//--------------------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(11, txId);     // Create 11 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("MuseumFloor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("Floor.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("bottom.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("front.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture
    loadTGA("top.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture
    loadTGA("right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);  //Use this texture
    loadTGA("left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[7]);  //Use this texture
    loadTGA("back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[8]);  //Use this texture
    loadTGA("Hay.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    //glBindTexture(GL_TEXTURE_2D, txId[8]);  //Use this texture
    //loadTGA("Marble.tga");
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

// -------------------------------------------------------------------------------
void skybox()
{
	// ----- Draw skybox front -------
	
	glEnable(GL_TEXTURE_2D);
	
	// Front Wall
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f(-256.0, -256.0, 256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0, 256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f( 256.0,  256.0, 256.0);
	glEnd();
	
	// Left Wall
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f(-256.0, -256.0,  256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f(-256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0,  256.0);
	glEnd();
	
	// Right Wall
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f(256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f(256.0, -256.0,  256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(256.0,  256.0,  256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(256.0,  256.0, -256.0);
	glEnd();
	
	// Back Wall
	glBindTexture(GL_TEXTURE_2D, txId[7]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f(-256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0, -256.0, -256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f( 256.0,  256.0, -256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
	glEnd();

	// Top Wall
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0,  256.0, -256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0,  256.0,  256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  256.0,  256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  256.0, -256.0);
	glEnd();
	
	//Bottom Wall
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);		glVertex3f( 256.0,  -256.0, -256.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f( 256.0,  -256.0,  256.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(-256.0,  -256.0,  256.0);
		glTexCoord2f(0.0, 1.0);		glVertex3f(-256.0,  -256.0, -256.0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
}


//-- Draw Museum -----------------------------------------------------------
void museum()
{
	glColor3f(0.93, 0.78, 0.18);
	glPushMatrix(); 	//left wall
        glTranslatef(-15, 0, -40);
        glScalef(1, 10, 30);
        glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.93, 0.78, 0.18);
	glPushMatrix(); 	//right wall
        glTranslatef(15, 0, -40);
        glScalef(1, 10, 30);
        glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.93, 0.78, 0.18);
	glPushMatrix(); 	//back wall
        glTranslatef(0, 0, -54.5);
        glScalef(30, 10, 1);
        glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.93, 0.78, 0.18);
	glPushMatrix(); 	//front wall left
        glTranslatef(-10.5, 0, -25);
        glScalef(-10, 10, 1);
        glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.93, 0.78, 0.18);
	glPushMatrix(); 	//front wall right
        glTranslatef(10.5, 0, -25);
        glScalef(-10, 10, 1);
        glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.5, 0.4, 0.25);
	glPushMatrix(); 	//roof
        glTranslatef(0, 5.5, -43);
        glScalef(-40, 1, 40);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, -0.90, -40);
    glScalef(-15, 0.1, 15);
		glPushMatrix();
	  
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glColor3f(1.0, 1.0, 1.0);
		
		glBegin(GL_QUADS);	//Floor
			glTexCoord2f(0., 0.);		glVertex3f(1.0, -0.99, -1.0);
			glTexCoord2f(2., 0.);		glVertex3f(1.0, -0.99, 1.0);
			glTexCoord2f(2., 2.);		glVertex3f(-1.0, -0.99, 1.0);
			glTexCoord2f(0., 2.);		glVertex3f(-1.0, -0.99, -1.0);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();
    
    
}

//------- Draw Bird ---------------------------------------------------------
void bird() 
{
	glPushMatrix();
	glTranslatef(-10.0, 0.5, -35.0);
	glScalef(2.0, 2.0, 2.0);
	
	glColor3f(0.53, 0.10, 0.10);
	glPushMatrix();		//Body
	glutSolidSphere(0.5, 36, 18); 
	glPopMatrix();
	
	glColor3f(0.53, 0.10, 0.10);
	glPushMatrix();		//Head
	glTranslatef(0.4, 0.4, 0);
	glutSolidSphere(0.3, 36, 18);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();		//Left Eye
	glTranslatef(0.5, 0.5, 0.25);
	glutSolidSphere(0.05, 36, 18); 
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();		//Right Eye
	glTranslatef(0.5, 0.5, -0.25);
	glutSolidSphere(0.05, 36, 18); 
	glPopMatrix();
	
	glColor3f(0.53, 0.10, 0.10);
	glPushMatrix();		//Right Wing
	glTranslatef(0.0, 0.1, -0.6);
	glTranslatef(0.35, 0.05, 0.5);	 //Rotate about origin
	glRotatef(theta, 1, 0, 0);		 //
	glTranslatef(-0.35, -0.05, -0.5);//
	glRotatef(20, 0, 1, 0);
	glScalef(0.7, 0.1, 1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.53, 0.10, 0.10);
	glPushMatrix();		//Left Wing
	glTranslatef(0.0, 0.1, 0.6);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0.35, 0.05, 0.5);	 //Rotate about origin
	glRotatef(-theta, 1, 0, 0);		 //
	glTranslatef(-0.35, -0.05, -0.5);//
	glRotatef(20, 0, 1, 0);
	glScalef(0.7, 0.1, 1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.3, 0.2, 0.2);
	glPushMatrix();		//Upper Beak
	glTranslatef(0.7, 0.4, 0.0);
	glTranslatef(-0.1, -0.025, -0.05);	//Rotate about origin
	glRotatef(theta, 0, 0, 1);			//
	glTranslatef(0.1, 0.025, 0.05);		//
	glScalef(0.2, 0.05, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.3, 0.2, 0.2);
	glPushMatrix();		//Lower Beak
	glTranslatef(0.7, 0.31, 0.0);
	glTranslatef(-0.1, -0.025, -0.05);	//Rotate about origin
	glRotatef(-theta, 0, 0, 1);			//
	glTranslatef(0.1, 0.025, 0.05);		//
	glScalef(0.2, 0.05, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();		//Left Leg
	glTranslatef(0.0, -0.5, 0.25);
	glTranslatef(0.035, 0.15, 0.035);	//Rotate about origin
	glRotatef(theta, 0, 0, 1);			//
	glTranslatef(-0.035, -0.15, -0.035);//
	glScalef(0.07, 0.3, 0.07);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();		//Right Leg
	glTranslatef(0.035, 0.15, 0.035);	//Rotate about origin
	glRotatef(-theta, 0, 0, 1);			//
	glTranslatef(-0.035, -0.15, -0.035);//
	glTranslatef(0.0, -0.5, -0.25);
	glScalef(0.07, 0.3, 0.07);
	glutSolidCube(1);
	glPopMatrix();
	
	glPopMatrix();
	
}

//----------- Draw Snowman ------------------------------------------------------------------

void snowman() 
{
	glPushMatrix();
	glTranslatef(10.0, -0.3, -40.0);
	glRotatef(-90, 0, 1, 0);
	glScalef(2.0, 2.0, 2.0);
	
	glColor3f(0.95, 0.95, 0.95);
	glPushMatrix();		//Lower body
	glutSolidSphere(0.4, 36, 18);
	glPopMatrix();
	
	glColor3f(0.95, 0.95, 0.95);
	glPushMatrix();		//Mid body
	glTranslatef(0.0, 0.6, 0.0);
	glutSolidSphere(0.3, 36, 18);
	glPopMatrix();
	
	glColor3f(0.95, 0.95, 0.95);
	glPushMatrix();		//Head
	glTranslatef(0.0, 1.0, 0.0);
	glutSolidSphere(0.2, 36, 18);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();		//Left eye
	glTranslatef(-0.1, 1.0, 0.18);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();		//Left eye
	glTranslatef(0.1, 1.0, 0.18);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.90, 0.50, 0.10);
	glPushMatrix();		//Nose
	glTranslatef(0.0, 0.95, 0.2);
	glScalef(0.03, 0.03, 0.2);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.25);
	glPushMatrix();		//Button 1
	glTranslatef(0.0, 0.7, 0.28);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.25);
	glPushMatrix();		//Button 2
	glTranslatef(0.0, 0.5, 0.29);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.25);
	glPushMatrix();		//Button 3
	glTranslatef(0.0, 0.3, 0.27);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.25);
	glPushMatrix();		//Button 4
	glTranslatef(0.0, 0.1, 0.39);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.2, 0.2, 0.25);
	glPushMatrix();		//Button 5
	glTranslatef(0.0, -0.1, 0.38);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.40, 0.20, 0.10);
	glPushMatrix();		//Left arm
	glTranslatef(-0.4, 0.8, 0.0);
	glRotatef(-45, 0, 0, 1);
	glTranslatef(0.25, 0.015, 0.015);	//Rotate about origin
	glRotatef(theta, 0, 0, 1);		 		//
	glTranslatef(-0.25, -0.015, -0.015);	//
	glScalef(0.5, 0.03, 0.03);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.40, 0.20, 0.10);
	glPushMatrix();		//Right arm
	glTranslatef(0.4, 0.8, 0.0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(-0.25, 0.015, 0.015);	//Rotate about origin
	glRotatef(-theta, 0, 0, 1);		 		//
	glTranslatef(0.25, -0.015, -0.015);	//
	glScalef(0.5, 0.03, 0.03);
	glutSolidCube(1);
	glPopMatrix();
	
	glPopMatrix();
}

void horse()
{
	glPushMatrix();
	glTranslatef(-10.0, -0.5, -30.0);
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//Body
	glScalef(0.6, 0.25, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//Front left leg
	glTranslatef(0.2, -0.2, 0.2);
	glScalef(0.1, 0.4, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//Front right leg
	glTranslatef(0.2, -0.2, -0.2);
	glScalef(0.1, 0.4, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//Back left leg
	glTranslatef(-0.2, -0.2, 0.2);
	glScalef(0.1, 0.4, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//back right leg
	glTranslatef(-0.2, -0.2, -0.2);
	glScalef(0.1, 0.4, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.4, 0.25, 0.2);
	glPushMatrix();		//Tail
	glTranslatef(-0.4, -0.05, 0.0);
	glRotatef(45, 0, 0, 1);	
	glTranslatef(0.2, 0.025, 0.025);		//
	glRotatef(theta, 0, 0, 1);			    //Rotate about the origin
	glTranslatef(-0.2, -0.025, -0.025);		//
	glScalef(0.4, 0.05, 0.05);
	glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.18, 0.2, 0.0);
	glRotatef(-57, 0, 0, 1);
	glRotatef(theta, 0, 0, 1);			    //Rotate about the origin
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//Neck
	glTranslatef(0.2, 0.2, 0.0);
	glScalef(0.15, 0.6, 0.15);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.4, 0.15, 0.0);
	glPushMatrix();		//Head
	glTranslatef(0.3, 0.45, 0.0);
	glScalef(0.3, 0.15, 0.15);
	glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();		//Left eye
	glTranslatef(0.25, 0.5, 0.07);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();		//Right eye
	glTranslatef(0.25, 0.5, -0.07);
	glutSolidSphere(0.02, 36, 18);
	glPopMatrix();
	
	glPopMatrix();
	
	glPopMatrix();
}

void hay()
{
	glPushMatrix();
	glTranslatef(-9.3, -0.8, -30.0);
	glScalef(2., 2.0, 2.0);
	
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
	glColor3f(1.0, 1.0, 1.0);
	
	glBegin(GL_QUADS);	//Front
		glTexCoord2f(0., 0.);		glVertex3f(-0.1, -0.1, -0.1);
		glTexCoord2f(1., 0.);		glVertex3f(0.1, -0.1, -0.1);
		glTexCoord2f(1., 1.);		glVertex3f(0.1, 0.1, -0.1);
		glTexCoord2f(0., 1.);		glVertex3f(-0.1, 0.1, -0.1);
	glEnd();
	
	glBegin(GL_QUADS); //Back
		glTexCoord2f(0., 0.);		glVertex3f(0.1, -0.1, 0.1);
		glTexCoord2f(1., 0.);		glVertex3f(-0.1, -0.1, 0.1);
		glTexCoord2f(1., 1.);		glVertex3f(-0.1, 0.1, 0.1);
		glTexCoord2f(0., 1.);		glVertex3f(0.1, 0.1, 0.1);
	glEnd();
	
	glBegin(GL_QUADS);	//Right
		glTexCoord2f(0., 0.);		glVertex3f(0.1, -0.1, -0.1);
		glTexCoord2f(1., 0.);		glVertex3f(0.1, -0.1, 0.1);
		glTexCoord2f(1., 1.);		glVertex3f(0.1, 0.1, 0.1);
		glTexCoord2f(0., 1.);		glVertex3f(0.1, 0.1, -0.1);
	glEnd();
	
	glBegin(GL_QUADS);	//Left
		glTexCoord2f(0., 0.);		glVertex3f(-0.1, -0.1, 0.1);
		glTexCoord2f(1., 0.);		glVertex3f(-0.1, -0.1, -0.1);
		glTexCoord2f(1., 1.);		glVertex3f(-0.1, 0.1, -0.1);
		glTexCoord2f(0., 1.);		glVertex3f(-0.1, 0.1, 0.1);
	glEnd();
	
	glBegin(GL_QUADS);	//Top
		glTexCoord2f(0., 0.);		glVertex3f(0.1, 0.1, -0.1);
		glTexCoord2f(1., 0.);		glVertex3f(0.1, 0.1, 0.1);
		glTexCoord2f(1., 1.);		glVertex3f(-0.1, 0.1, 0.1);
		glTexCoord2f(0., 1.);		glVertex3f(-0.1, 0.1, -0.1);
	glEnd();
	
	glBegin(GL_QUADS);	//Bottom
		glTexCoord2f(0., 0.);		glVertex3f(0.1, -0.1, -0.1);
		glTexCoord2f(1., 0.);		glVertex3f(0.1, -0.1, 0.1);
		glTexCoord2f(1., 1.);		glVertex3f(-0.1, -0.1, 0.1);
		glTexCoord2f(0., 1.);		glVertex3f(-0.1, -0.1, -0.1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
}

void horseAndHay()
{
	glPushMatrix();
	horse();
	hay();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void chalice()
{
    glPushMatrix();
      
		glColor3f(1.0, 0.8, 0.0);
		glScalef(0.03, 0.03, 0.03);
		glTranslatef(0.0, -30.0, -1300.0);
		glRotatef(180, 0, 1, 0);
    
        float wx[N], wy[N], wz[N];
        float thetaVase = 10;

        for(int j = 0; j < 36; j++) //36 slices in 10 deg steps
        {
			for(int i = 0; i < N; i++)
			{
				wx[i] =(vx[i] * cos(thetaVase * M_PI / 180.0)) + (vz[i] * sin(thetaVase * M_PI / 180.0));
				wy[i] = vy[i];
				wz[i] = (-vx[i] * sin(thetaVase * M_PI / 180.0)) + ( vz[i] * cos(thetaVase * M_PI / 180.0));

				glBegin(GL_TRIANGLE_STRIP); //CREATE TRIABLE STRIP USING V, W
				
				 if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1], vx[i-1], vy[i-1], vz[i-1], vx[i], vy[i], vz[i]);
                glTexCoord2f((float)j/36, (float)i/(N-1)); //Texture map
                glVertex3f(vx[i], vy[i], vz[i]);
				 
				 if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1], vx[i], vy[i], vz[i], wx[i], wy[i], wz[i]);
                glTexCoord2f((float)(j+1)/36, (float)i/(N-1)); //Texture map
                glVertex3f(wx[i], wy[i], wz[i]);
			}
			glEnd();
        
			for (int i = 0; i < N; i++)
			{
				vx[i] = wx[i];
				vy[i] = wy[i];
				vz[i] = wz[i];
			}
        }
        
    glPopMatrix();
}

//----------- Draw Floor ---------------------------------------------------------------------

void floor()
{
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glColor3d(0.6, 0.25, 0.25);  //replace with a texture

    glBegin(GL_QUADS);
         glTexCoord2f(0.0, 160.0);
             glVertex3f(-150, -1, -150);
         glTexCoord2f(0.0, 0.0);
             glVertex3f(-150, -1, 150);
         glTexCoord2f(160.0, 0.0);
            glVertex3f(150, -1, 150);
         glTexCoord2f(160.0, 160.0);
            glVertex3f(150, -1, -150);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

// -- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
    ifstream fp_in;
    int num, ne;

    fp_in.open(fname, ios::in);
    if(!fp_in.is_open())
    {
        cout << "Error opening mesh file" << endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n');                //ignore first line
    fp_in >> nvrt >> ntri >> ne;                // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

    for(int i=0; i < nvrt; i++)                         //read vertex list
        fp_in >> x[i] >> y[i] >> z[i];

    for(int i=0; i < ntri; i++)                         //read polygon list
    {
        fp_in >> num >> t1[i] >> t2[i] >> t3[i];
        if(num != 3)
        {
            cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
            exit(1);
        }
    }

    fp_in.close();
    cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
    float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
    float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
    float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
    float nx, ny, nz;
    nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}

//--------draws the mesh model of the cannon----------------------------
void drawCannon()
{

    //Construct the object model here using triangles read from OFF file
    glBegin(GL_TRIANGLES);
        for(int tindx = 0; tindx < ntri; tindx++)
        {
           normal(tindx);
           glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
           glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
           glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
        }
    glEnd();
}

void myTimerCannon(int value)
{
	if (cannonBall_x <= 400)
	{
		x1 += 1;
		cannonBall_x += 4;
	}
	else 
	{
		cannonBall_x = 0; //reset back to origin
		x1 = 0;	//reset x value
	}
	glutPostRedisplay();
    glutTimerFunc(25, myTimerCannon, 0);   //(time in ms, what to call, counter value)
}


// --------- Draws the cannon model and its planar shadow -------------
void cannon()
{
    float light[] = {0, 400, 300, 1};
    float shadowMat[16] = { 400,0,0,0, 0,0,-300,-1,
                            0,0,400,0, 0,0,0,400 };
                            
    glLightfv(GL_LIGHT0, GL_POSITION, light);   
    
    glPushMatrix();
    glTranslatef(-10.0, 0.0, -40.0);
      
    // DRAW CANNON PLANAR SHADOW OBJECT
    glDisable(GL_LIGHTING);

    glPushMatrix();
	glScalef(0.02, 0.02, 0.02);
	glTranslatef(0.0, -50.0, -200.0);
    glTranslatef(0, 0.1, 0); //Raise shadow off ground slightly otherwise its glitchy
    
            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-20, 30, 0);
                glRotatef(30.0, 0, 0, 1);
                glTranslatef(20, -30, 0);
                glColor4f(0.2, 0.2, 0.2, 1.0);
                drawCannon();
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-10,5,17);
                glScalef(80,10,6);
                glColor4f(0.2, 0.2, 0.2, 1.0);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-20,25,17);
                glScalef(40,30,6);
                glColor4f(0.2, 0.2, 0.2, 1.0);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-10,5,-17);
                glScalef(80,10,6);
                glColor4f(0.2, 0.2, 0.2, 1.0);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-20,25,-17);
                glScalef(40,30,6);
                glColor4f(0.2, 0.2, 0.2, 1.0);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glutSolidSphere(5, 36, 18);
            glPopMatrix();
    glPopMatrix();
    
    // DRAW CANNON OBJECT
    glEnable(GL_LIGHTING);
    glPushMatrix();
	glScalef(0.02, 0.02, 0.02);
	glTranslatef(0.0, -50.0, -200.0);
    
		glColor3f(1.0, 0.2, 0);
		glPushMatrix();
		glTranslatef(-20, 30, 0);
		glRotatef(30, 0, 0, 1);
		glTranslatef(20, -30, 0);
		drawCannon();
		glPopMatrix();
		
		glColor3f(0.4, 0.2, 0);
		glPushMatrix();
		glTranslatef(-10, 5, 17);
		glScalef(80, 10, 6);
		glutSolidCube(1);
		glPopMatrix();
		
		glColor3f(0.4, 0.2, 0);
		glPushMatrix();
		glTranslatef(-20, 25, 17);
		glScalef(40, 30, 6);
		glutSolidCube(1);
		glPopMatrix();
		
		glColor3f(0.4, 0.2, 0);
		glPushMatrix();
		glTranslatef(-10, 5, -17);
		glScalef(80, 10, 6);
		glutSolidCube(1);
		glPopMatrix();
		
		glColor3f(0.4, 0.2, 0);
		glPushMatrix();
		glTranslatef(-20, 25, -17);
		glScalef(40, 30, 6);
		glutSolidCube(1);
		glPopMatrix();
		
		glColor3f(0.0, 0.0, 0);
		glPushMatrix();
		
		float y_value = ((-0.02 * x1 * x1) + (1.5 * x1) + 1);
		glTranslatef(38.88 + cannonBall_x, 64 + y_value, 0);
		
		glutSolidSphere(5, 36, 18);
		glPopMatrix();
		
	glPopMatrix();
	
	glPopMatrix();
               
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 0.5*sin(angle);
        eye_z += 0.5*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 0.5*sin(angle);
        eye_z -= 0.5*cos(angle);
    }
    
    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    
    //---- Collision detection of museum walls ------------------------------------
    
	//Back wall collision
	if((eye_x > -15.0) && (eye_x < 15.0) && (eye_z > -56.5) && (eye_z < -53.5)) {
		eye_x = coll_eye_x;
		eye_z = coll_eye_z;
	}
	//Front right wall collision
	if((eye_x > 5) && (eye_x < 15) && ((eye_z < -24) && (eye_z > -26))) {
		eye_x = coll_eye_x;
		eye_z = coll_eye_z;
	}
	//Front left wall collision
	if((eye_x > -15) && (eye_x < -5) && ((eye_z < -24) && (eye_z > -26))) {
		eye_x = coll_eye_x;
		eye_z = coll_eye_z;
	}
	//Right wall collision
	if((eye_x > 14.0) && (eye_x < 16.0) && ((eye_z > -68) && (eye_z < -32))) {
		eye_x = coll_eye_x;
		eye_z = coll_eye_z;
	}
	//Left wall collision
	if((eye_x > -16.0) && (eye_x < -14.0) && ((eye_z > -68) && (eye_z < -32))) {
		eye_x = coll_eye_x;
		eye_z = coll_eye_z;
	}
	
	coll_eye_x = eye_x;
	coll_eye_z = eye_z;
    
    glutPostRedisplay();
}

//--------------------------------------------------------------------------------

void initialise()
{
	float white[4] = {1.0, 1.0, 1.0, 1.0};
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	//loadGLTextures();
	
    loadTexture();
    glEnable(GL_TEXTURE_2D);
    loadMeshFile("Cannon.off");
    glEnable(GL_DEPTH_TEST);
    
    // -- Lighting stuff ----
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    // --------------
    
    glDisable(GL_TEXTURE_2D);
}


//--------------------------------------------------------------------------------

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45., 1., 1., 1000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);
    
    //Added this light stuff
    float light_pos[] = {20, 20, 0, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    //-------------------
    
    museum();
    floor();
    bird();
    snowman();
	horseAndHay();
    skybox();
    cannon();
    
    chalice();
    
    glFlush();
    glutSwapBuffers();
}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (900, 900);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("The Museum");
   initialise();

   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutTimerFunc(20, myTimer, 0);
   glutTimerFunc(20, myTimerCannon, 0);
   
   glutMainLoop();
   return 0;
}
