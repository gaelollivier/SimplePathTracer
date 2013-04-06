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
#include <sstream>


#include <glm/glm.hpp>

using namespace glm;

class RenderingSession;
class Renderer;
class Scene;
class Node;
class Camera;
class Light;
class Material;
class Object;
class Sphere;
class Plane;

// Operators for redirection to standard streams
std::ostream& operator<<(std::ostream& stream, const vec2& v);
std::ostream& operator<<(std::ostream& stream, const vec3& v);

// Utilities for debuging
namespace Debug {
    std::string indent(int level);
    
    namespace Detail {
    
        template<class T>
        std::string ToString(const T& value, uint32_t level) {
            std::stringstream str;
            str << value;
            return str.str();
        }
        
        template<>
        std::string ToString(const bool& value, uint32_t level);
        template<>
        std::string ToString(const RenderingSession& session, uint32_t level);
        template<>
        std::string ToString(const Renderer& renderer, uint32_t level);
        template<>
        std::string ToString(const Scene& scene, uint32_t level);
        template<>
        std::string ToString(const Material& material, uint32_t level);
        template<>
        std::string ToString(const Node& node, uint32_t level);
        template<>
        std::string ToString(const Camera& node, uint32_t level);
        template<>
        std::string ToString(const Light& node, uint32_t level);
        template<>
        std::string ToString(const Object& node, uint32_t level);
        template<>
        std::string ToString(const Sphere& node, uint32_t level);
        template<>
        std::string ToString(const Plane& node, uint32_t level);
        
        std::string ToStringNodeChilds(const Node& node, uint32_t level);
        
        template<class T>
        std::string ToStringPtr(const T* value, uint32_t level=0) {
            std::stringstream str;
            if (value)
                str << ToString(*value, level);
            else
                str << "None";
            return str.str();
        }
        
    }
    
    template<class T>
    std::string ToString(const T& value) {
        return Detail::ToString(value, 0);
    }
    
    std::string TimeToString(float seconds, bool msPrecision=false);

}

template<class T>
void d(T value) {
    std::cout << value << std::endl;
}

#endif /* defined(__SimplePathTracer__Debug__) */
