//
//  Ray.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Ray__
#define __SimplePathTracer__Ray__

#include "glm/glm.hpp"
using namespace glm;

// Include debug here, because it is included in almost all the project :)
#include "Debug.h"

class Object;

struct Ray {
    Ray(vec3 origin=vec3(), vec3 direction=vec3());
    
    vec3 origin;
    vec3 direction;
    uint32_t objectId;
};

#endif /* defined(__SimplePathTracer__Ray__) */
