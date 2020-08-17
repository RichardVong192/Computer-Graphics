/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray (pos, dir).
*/
float Cylinder::intersect(glm::vec3 pos, glm::vec3 dir) 		//Refer to lec 8, slide 38
{
    float a = (dir.x *dir.x) + (dir.z * dir.z);				
    float b = 2 * ((dir.x *(pos.x - center.x) + (dir.z * (pos.z - center.z))));
    float c = ((pos.x - center.x) * (pos.x - center.x)) + ((pos.z - center.z) * (pos.z - center.z)) - (radius * radius);
    float disciminant = b*b -(4 * a * c) ;

    if(fabs(disciminant) < 0.001 || disciminant < 0.0) {
		 return -1.0;
    }

    float t1 = (-b - sqrt(disciminant)) / (2*a);
    float t2 = (-b + sqrt(disciminant)) / (2*a);

    if(fabs(t1<0.01)){	// if they dont intersect, set to negative (imaginary roots)
		t1=-1; 	
	 }
	if(fabs(t2<0.01)) {	// if they dont intersect, set to negative (imaginary roots)
		t2=-1;	
	 }

	if(fabs(t1) < 0.001 )
    {
        if (t2 > 0) return t2;
        else t1 = -1.0;	// if they dont intersect, set to negative
    }
    if(fabs(t2) < 0.001 ) t2 = -1.0;
    
    float m;
    if (t1 < t2) {
		m = pos.y + dir.y * t1;
	} else {
		m = pos.y + dir.y * t2;
	}
    
    return (m >= center.y) && (m <= (center.y + height))? t1 : -1.0;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)		//Refer to lec 8, slide 38
{
    glm::vec3 closestPt = p - center;
	glm::vec3 n = glm::vec3 (closestPt.x, 0, closestPt.z); //un-normalised
	n = glm::normalize(n); //normalize
	return n;
}
