//
//  Material.h
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#ifndef __SimplePathTracer__Material__
#define __SimplePathTracer__Material__

#include "glm/glm.hpp"

using namespace glm;

class Material {
public:
    
    Material(void);
    Material(const vec3& color);
    
    virtual ~Material(void);
    
    vec3    getColor(void) const;
    void    setColor(const vec3& color);
    
    float   getReflection(void) const;
    void    setReflection(float reflection);
    
    float   getRefraction(void) const;
    void    setRefraction(float refraction);
    
    float   getDiffuse(void) const;
    void    setDiffuse(float diffuse);
    
    float   getSpecular(void) const;
    void    setSpecular(float specular);
    
    float   getShininess(void) const;
    void    setShininess(float shininess);
    
    float   getGlossiness(void) const;
    void    setGlossiness(float glossiness);
    
private:
    vec3    _color;
    float   _reflection;
    float   _refraction;
    float   _diffuse;
    float   _specular;
    float   _shininess;
    float   _glossiness;
};

#endif /* defined(__SimplePathTracer__Material__) */
