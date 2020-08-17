/*----------------------------------------------------------
* COSC363  Ray Tracer Assignment
*
*  Cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_CYLINDER
#define H_CYLINDER

#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Class that defines cylinder object
 */
class Cylinder : public SceneObject
{

private:
    glm::vec3 center;
    float radius;
    float height;

public:
	Cylinder()
		: center(glm::vec3(0)), radius(1), height(1) {}; //Default constructor creates a unit sphere
	
    Cylinder(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {};

	float intersect(glm::vec3 posn, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);


};

#endif //!H_CYLINDER
