//
//  Debug.cpp
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#include "Debug.h"

std::ostream& operator<<(std::ostream& stream,const vec3& v) {
    stream << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}