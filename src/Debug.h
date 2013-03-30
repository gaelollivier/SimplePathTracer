//
//  Debug.h
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#ifndef __SimplePathTracer__Debug__
#define __SimplePathTracer__Debug__

#include <iostream>

#include <glm/glm.hpp>

using namespace glm;

std::ostream& operator<<(std::ostream& stream,const vec3& v);

template<class T>
void d(T value) {
    std::cout << value << std::endl;
}

#endif /* defined(__SimplePathTracer__Debug__) */
