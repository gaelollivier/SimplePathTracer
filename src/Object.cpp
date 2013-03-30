//
//  Object.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Object.h"

Object::Object(vec3 position, Material* material) :
    Node(position), _material(material)
{
}

Object::~Object(void) {
}

void Object::setMaterial(Material* material) {
    _material = material;
}

Material* Object::getMaterial(void) const {
    return _material;
}

bool Object::hasBoundingBox(void) const {
    return true;
}