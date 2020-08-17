/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray (pos, dir).
*/
float Cone::intersect(glm::vec3 pos, glm::vec3 dir)		// Refer to lec 8 slide 44
{
    float coneBaseAxisX = pos.x - center.x;		//Values of the cone's equation
    float coneBaseAxisZ = pos.z - center.z;
    float tan = radius / height;
    float coneBaseAxisY = height - pos.y + center.y;
    
    float a = (dir.x * dir.x) + (dir.z * dir.z) -  ((tan * tan) * (dir.y * dir.y));	//Values of the discriminant
    float b = 2 * (dir.x * coneBaseAxisX + dir.z * coneBaseAxisZ) + 2 * (tan * tan) * (dir.y * coneBaseAxisY);
    float c = (coneBaseAxisX * coneBaseAxisX) + (coneBaseAxisZ * coneBaseAxisZ) - (tan * tan) * (coneBaseAxisY * coneBaseAxisY);
    float disciminant = b*b -(4 * a * c) ;


    if(fabs(disciminant) < 0.001 || disciminant < 0.0) {
		 return -1.0;
    }

    float t1 = (-b - sqrt(disciminant)) / (2 * a);
    float t2 = (-b + sqrt(disciminant)) / (2 * a);
    

	if(fabs(t1) < 0.001 )
    {
        if (t2 > 0) return t2;
        else t1 = -1.0;	// if they dont intersect, set to negative
    }
    if(fabs(t2) < 0.001 ) t2 = -1.0;
    
    float m;
    if (t1 < t2) {
		m = pos.y + dir.y * t2;
	} else {
		m = pos.y + dir.y * t1;
	}
    
    return (m >= center.y) && (m <= (center.y + height))? t1 : -1.0;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cone::normal(glm::vec3 p)		//Refer to lec 8, slide 44
{
	float alpha = atan((p.x - center.x)/(p.z-center.z));
	float theta = atan(radius/height);
	glm::vec3 n = glm::vec3(sin(alpha)*cos(theta), sin(theta), cos(alpha)*cos(theta)); //un-normalised
	
    n = glm::normalize(n); //normalize
    return n;
}
