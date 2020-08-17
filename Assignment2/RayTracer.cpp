/*==================================================================================
* COSC 363  Computer Graphics (2020)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf, Lab08.pdf for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"
#include "Cone.h"
#include "Cylinder.h"

TextureBMP texture;
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	//glm::vec3 backgroundCol(0.4, 0.7, 1);			//Background colour
	glm::vec3 backgroundCol(1,1,1);
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found
	
	//------ CHECKERED PATTERN FLOOR ----------
	if (ray.index == 4)
	{
		int stripeWidth = 5;
		int modx = (int)((ray.hit.x + 50) / stripeWidth) % 2;
		int modz = (int)((ray.hit.z) / stripeWidth) % 2;
		
		if ((modx && modz) || (!modx && !modz)) {
			color = glm::vec3(0.5, 0.5, 1); //violet color
			obj->setColor(color);
		} else {
			color = glm::vec3(1, 0.5, 0); //orange color
			obj->setColor(color);
		}
	}
	
	if (ray.index == 3)	//Procedural sine pattern
	{
		float amp = 1;
		float ix = ((ray.hit.x) * M_PI)/5;
		float iy = amp * sin(ix) + amp;
		float hity = fabs(ray.hit.y);
		float checky = fmod(hity, 10);
		if (checky < 5) {
			ix += M_PI;
		}
		if (fmod(iy, (amp*2)) < fmod(hity, (amp*2))){
			color = glm::vec3(1, 1, 0);
			obj->setColor(color);
		 }
		else {
			color = glm::vec3(0, 0, 0);
			obj->setColor(color);
		}
		
	}
	
	
	
	//	Non planar texture mapping
	//	Refer to lec 8, slide 31
    if(ray.index == 13) {	//Index of the sphere
        glm::vec3 dir = glm::normalize(ray.hit - glm::vec3(6.0, 16.0, -80.0));

        float u = (atan2(dir.z, dir.x) / (2 * M_PI)) + 0.5;
        float v =  (asin(dir.y) / M_PI) + 0.5;

        color = texture.getColorAt(u, v);
        obj->setColor(color);

    }
	

	color = obj->lighting(lightPos, -ray.dir, ray.hit); //light position, view vector, point of intersection (where color is color value is computed)
	
	// ------ Shadow stuff -----------
	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);
	
	shadowRay.closestPt(sceneObjects);
	
	float lightDist = glm::length(lightVec);
	
	if ((shadowRay.index > -1) && (shadowRay.dist < lightDist)) 
	{
		if(shadowRay.index == 2 || shadowRay.index == 1) //Index of the object I want to have lighter shadow
		{
			color = 0.6f * obj->getColor();
		}
		else 
		{
			color = 0.2f * obj->getColor(); //0.2 = ambient scale factor
		}
		
	}
	
	if(obj->isTransparent() && step < MAX_STEPS) {	//Make object transparent
		Ray transparentRay(ray.hit, ray.dir);
		glm::vec3 transparentCol = trace(transparentRay, 1);
		color = color + (0.9f* transparentCol);
	}
	
	if (obj->isReflective() && step <MAX_STEPS) //Reflection
	{
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho*reflectedColor);
	}
	
	if (obj->isRefractive() && step < MAX_STEPS) {
		
		float eta = 1/ obj->getRefractiveIndex(); 	//Eta value for sphere refraction
		
		glm::vec3 n = obj->normal(ray.hit);
		glm::vec3 g = glm::refract(ray.dir, n, eta);
		Ray refrRay(ray.hit, g);
		refrRay.closestPt(sceneObjects);
		glm::vec3 m = obj->normal(refrRay.hit);
        glm::vec3 h = glm::refract(g, -m, 1.0f/eta);
        
        Ray refrRay2(refrRay.hit, h);
        glm::vec3 refracCol = trace(refrRay2, step + 1);
        
        color = color + obj->getRefractionCoeff() * refracCol;
	}
	
	//FOG
	int z2 = -200;
	int z1 = -50;
	float t = (ray.hit.z - z1) / (z2 - z1);					//interpolation parameter
	color = (1-t)*color + t*glm::vec3(1.0, 1.0, 1.0);		//Modify color

	return color;
}

//----------ANTI-ALIASING/SUPERSAMPLING--------
glm::vec3 antiAliasing(float xpos, float ypos, glm::vec3 eye, float pixelSize) {
	float upperQuadsCenterPt = (pixelSize * 0.75f);
	float lowerQuadsCenterPt = (pixelSize * 0.25f);
	
	glm::vec3 color = glm::vec3(0, 0, 0);	//Initalise color 
	
	// Refer to lab 7, Fig 2
	// For how to calculate rays
	
    // Bottom left
    Ray ray = Ray(eye, glm::vec3(xpos + lowerQuadsCenterPt, ypos + lowerQuadsCenterPt, -EDIST));
    color += trace(ray, 1);
    
    // Bottom right
    ray = Ray(eye, glm::vec3(xpos + upperQuadsCenterPt, ypos + lowerQuadsCenterPt, -EDIST));
    color += trace(ray, 1);
    
    // Top right 
    ray = Ray(eye, glm::vec3(xpos + upperQuadsCenterPt, ypos + upperQuadsCenterPt, -EDIST));
    color += trace(ray, 1);

    // Top left
    ray = Ray(eye, glm::vec3(xpos + lowerQuadsCenterPt, ypos + upperQuadsCenterPt, -EDIST));
    color += trace(ray, 1);
	
	//return average of the colors
    return color *= 0.25f;
	
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for(int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

		    glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray

			glm::vec3 col = antiAliasing(xp, yp, eye, cellX);	//Anti Aliasing
			
			//Ray ray = Ray(eye, dir);			//Uncomment these two lines to show scene with no antialiasing
		    //glm::vec3 col = trace (ray, 1); 	//Trace the primary ray and get the colour value
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 15.0);	//Big dark blue sphere index 0
	sphere1->setColor(glm::vec3(0, 0, 1)); 		  						//Set colour to blue
	//sphere1->setSpecularity(false);									//Disable specular reflections
	sphere1->setReflectivity(true, 0.8);								//Make it reflective							
	
	Sphere *sphere2 = new Sphere(glm::vec3(-13.0, -10.0, -70.0), 4.0);		//Transparent sphere 
	sphere2->setTransparency(true, 0.8);
	sphere2->setReflectivity(true, 0.05);
	sphere2->setColor(glm::vec3(0.2, 0, 0));

	Sphere *sphere3 = new Sphere(glm::vec3(10.0, -10.0, -70.0), 5.0);	//Refractive sphere 
	sphere3->setRefractivity(true, 0.8, 1.01); 							//true, coeff-recfraction, refractive index
	sphere3->setColor(glm::vec3(0.0, 0.0, 0.0));
	
	Sphere *sphere4 = new Sphere(glm::vec3(-23.0, 22, -120.0), 6.0);	//White sphere
	//sphere4->setColor(glm::vec3(1, 1, 0));
	
	Cone *cone = new Cone(glm::vec3(8.0, 5.0, -60.0), 2, 5); 	//Cone 
	cone->setColor(glm::vec3(0, 1, 0));
	
	Cylinder *cylinder = new Cylinder(glm::vec3(16, -10, -90), 3, 15.0);	//Cylinder
	cylinder->setColor(glm::vec3(0.6, 0.2, 1));
	
	Sphere *sphere5 = new Sphere(glm::vec3(6.0, 16.0, -80.0), 3.0);	//Earth sphere
	sphere5->setColor(glm::vec3(0, 1, 1));
	
	
	//----------- Floor Plane -------------------------------
	Plane *plane = new Plane(glm::vec3(-200., -15, -40),	//Point A
							glm::vec3(200., -15, -40),		//Point B
							glm::vec3(200., -15, -200),	    //Point C
							glm::vec3(-200., -15, -200));	//Point D
							
	//--------------Cube Planes --------------------------
	Plane *cubeBottom = new Plane(glm::vec3(-5., -14, -60),	//Point A
							glm::vec3(0., -14, -60),		//Point B
							glm::vec3(0., -14, -70),	    //Point C
							glm::vec3(-5., -14, -70));		//Point D
	cubeBottom->setColor(glm::vec3(1, 0.2, 0.6));
	
	Plane *cubeTop = new Plane(glm::vec3(-5., -11, -60),	//Point A	
							glm::vec3(0., -11, -60),		//Point B
							glm::vec3(0., -11, -70),	    //Point C
							glm::vec3(-5., -11, -70));		//Point D
	cubeTop->setColor(glm::vec3(1, 0.2, 0.6));
	
	Plane *cubeFront = new Plane(glm::vec3(-5., -14, -60),	//Point A
							glm::vec3(0., -14, -60),		//Point B
							glm::vec3(0., -11, -60),	    //Point C
							glm::vec3(-5., -11, -70));		//Point D
	cubeFront->setColor(glm::vec3(1, 0.2, 0.6));
	
	Plane *cubeBack = new Plane(glm::vec3(-5., -14, -70),	//Point A
							glm::vec3(0., -14, -70),		//Point B
							glm::vec3(0., -11, -70),	    //Point C 
							glm::vec3(-5., -11, -70));		//Point D
	cubeBack->setColor(glm::vec3(1, 0.2, 0.6));
	
	Plane *cubeRight = new Plane(glm::vec3(0., -14, -70),	//Point A
							glm::vec3(0., -14, -60),		//Point B
							glm::vec3(0., -11, -60),	    //Point C 
							glm::vec3(0., -11, -70));		//Point D
	cubeRight->setColor(glm::vec3(1, 0.2, 0.6));
	
	Plane *cubeLeft = new Plane(glm::vec3(-5., -14, -60),	//Point A
							glm::vec3(-5., -14, -70),		//Point B
							glm::vec3(-5., -11, -70),	    //Point C 
							glm::vec3(-5., -11, -60));		//Point D
	cubeLeft->setColor(glm::vec3(1, 0.2, 0.6));
	
	texture = TextureBMP("earth.bmp");
	
	
	sceneObjects.push_back(sphere1);	//Index 0
	sceneObjects.push_back(sphere2);	//Index 1
	sceneObjects.push_back(sphere3);	//Index 2
	sceneObjects.push_back(sphere4);	//Index 3
	sceneObjects.push_back(plane);		//Index 4
	
	sceneObjects.push_back(cubeBottom);		//Index 5
	sceneObjects.push_back(cubeTop);		//Index 6
	sceneObjects.push_back(cubeFront);		//Index 7
	sceneObjects.push_back(cubeBack);		//Index 8
	sceneObjects.push_back(cubeRight);		//Index 9
	sceneObjects.push_back(cubeLeft);		//Index 10
	
	sceneObjects.push_back(cone);			//Index 11	
	sceneObjects.push_back(cylinder);		//Index 12
	sceneObjects.push_back(sphere5);		//Index 13

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
