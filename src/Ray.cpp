//
//  Ray.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Ray.h"

Ray::Ray(vec3 origin, vec3 direction) :
    origin(origin), direction(direction), objectId(0)
{
}