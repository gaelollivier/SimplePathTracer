//
//  Material.cpp
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#include "Material.h"

Material::Material() :
    _color(), _reflection(0), _refraction(0),
    _diffuse(1), _specular(1), _shininess(100.0),
    _glossiness(0.3)
{
}

Material::Material(const vec3& color) :
_color(color), _reflection(0), _refraction(0),
_diffuse(1), _specular(1), _shininess(100.0),
_glossiness(0.0)
{
}

Material::~Material(void) {
}

vec3 Material::getColor(void) const {
    return _color;
}

void Material::setColor(const vec3& color) {
    _color = color;
}

float Material::getReflection(void) const {
    return _reflection;
}

void Material::setReflection(float reflection) {
    _reflection = reflection;
}

float Material::getRefraction(void) const {
    return _refraction;
}

void Material::setRefraction(float refraction) {
    _refraction = refraction;
}

float Material::getDiffuse(void) const {
    return _diffuse;
}

void Material::setDiffuse(float diffuse) {
    _diffuse = diffuse;
}

float Material::getSpecular(void) const {
    return _specular;
}

void Material::setSpecular(float specular) {
    _specular = specular;
}

float Material::getShininess(void) const {
    return _shininess;
}

void Material::setShininess(float shininess) {
    _shininess = shininess;
}

float Material::getGlossiness(void) const {
    return _glossiness;
}

void Material::setGlossiness(float glossiness) {
    _glossiness = glossiness;
}