//
//  PythonExtensions.h
//  SimplePathTracer
//
//  Created by gael on 31/03/13.
//
//

#ifndef __SimplePathTracer__PythonExtensions__
#define __SimplePathTracer__PythonExtensions__

#include <sstream>
#include <glm/glm.hpp>

using namespace glm;

#include "Debug.h"

namespace Extensions {
    
    //
    // Basic operators, to be used with vectors for instance
    //
    namespace Operators {
        
        template<class T1, class T2>
        T1 Div(const T1& v, T2 value) {
            return v / value;
        }
        
        template<class T1, class T2>
        T1 Mult(const T1& v, T2 value) {
            return v * value;
        }
        
        template<class T1, class T2>
        T1 Add(const T1& v, T2 value) {
            return v + value;
        }
        
        template<class T1, class T2>
        T1 Sub(const T1& v, T2 value) {
            return v - value;
        }        
        
        
    }
    
}

#endif /* defined(__SimplePathTracer__PythonExtensions__) */
