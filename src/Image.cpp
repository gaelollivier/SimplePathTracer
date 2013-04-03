//
//  Image.cpp
//  SimplePathTracer
//
//  Created by gael on 03/04/13.
//
//

#include "Image.h"

#include <SFML/Graphics.hpp>
#include "Debug.h"

Image::Image(void) :
    _size(), _data()
{
}

Image::~Image(void) {
}

void Image::clear(void) {
    _data.clear();
    _data.resize(_size.x * _size.y);
}

void Image::setSize(const vec2& size) {
    _size = size;
    _data.resize(_size.x * _size.y);
}

vec2 Image::getSize(void) const {
    return _size;
}

const std::vector<vec3>& Image::getData(void) const {
    return _data;
}

vec3 Image::getPixel(vec2 position) const {
    uint32_t index = (uint32_t)(position.y * _size.x + position.x);
    return _data[index];
}

void Image::setPixel(vec2 position, vec3 value) {
    if (position.x < _size.x && position.y < _size.y) {
        uint32_t index = (uint32_t)(position.y * _size.x + position.x);
        _data[index] = value;
    }
}
