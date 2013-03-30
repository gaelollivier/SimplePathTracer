//
//  BoundingBox.h
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#ifndef __SimplePathTracer__BoundingBox__
#define __SimplePathTracer__BoundingBox__

#include "Ray.h"

struct BoundingBox {
    
    BoundingBox(void);
    BoundingBox(vec3 min, vec3 max);
    ~BoundingBox(void);
    
    vec3    getCenter(void) const;
    bool    intersectWithBox(const BoundingBox& other) const;
    float   intersectWithRay(const Ray& ray, bool* isInBox=NULL) const;
    
    vec3 min;
    vec3 max;
};

#endif /* defined(__SimplePathTracer__BoundingBox__) */
