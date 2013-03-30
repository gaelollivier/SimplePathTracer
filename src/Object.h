//
//  Object.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Object__
#define __SimplePathTracer__Object__

#include "Node.h"
#include "Material.h"
#include "Ray.h"

using namespace glm;

class Object : public Node {
public:
    
    Object(vec3 position=vec3(), Material* material=NULL);
    virtual ~Object(void);
    
    void setMaterial(Material* material);
    Material* getMaterial(void) const;
    
    virtual bool hasBoundingBox(void) const;
    
    virtual vec3 normalAtPoint(const vec3& point) = 0;
    
private:
    Material*   _material;
};

#endif /* defined(__SimplePathTracer__Object__) */
