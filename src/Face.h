//
//  Face.h
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#ifndef __SimplePathTracer__Face__
#define __SimplePathTracer__Face__

#include "Object.h"

class Face : public Object {
public:
    
    Face(vec3 p0, vec3 p1, vec3 p2, Material* material=NULL);
    virtual ~Face(void);
    
    void setNormals(const vec3& n1, const vec3& n2, const vec3& n3);
    void computeNormal(void);
    
    void setTextureCoords(const vec2& t1, const vec2& t2, const vec2& t3);
    
    virtual void update(void);
    
    virtual float intersectWithRay(const Ray& ray);
    
    virtual vec3 normalAtPoint(const vec3& point);
    
private:
    vec3    _points[3];
    vec3    _absolutePoints[3];
    vec3    _normals[3];
    vec2    _textureCoords[3];
};

#endif /* defined(__SimplePathTracer__Face__) */
