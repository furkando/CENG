#ifndef _DEFS_H_
#define _DEFS_H_


#include "Vector3f.h"

class Scene;

/* Structure to hold return value from ray intersection routine. 
This should hold information related to the intersection point, 
for example, coordinate of the intersection point, surface normal at the intersection point etc. 
Think about the variables you will need for this purpose and declare them here inside of this structure. */
typedef struct ReturnVal
{
	Vector3f normal;
	Vector3f point;
	float t;
	int matIndex;
} ReturnVal;

//
// The global variable through which you can access the scene data
//
extern Scene *pScene;

#endif
