//
//  Debug.cpp
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#include "Debug.h"

#include <iomanip>

#include "RenderingSession.h"
#include "Renderer.h"
#include "Scene.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

std::ostream& operator<<(std::ostream& stream, const vec2& v) {
    stream << "vec2(" << v.x << ", " << v.y << ")";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const vec3& v) {
    stream << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}

std::string Debug::indent(int level) {
    std::stringstream str;
    for (uint32_t i = 0; i < level; ++i)
        str << "\t";
    return str.str();
}

template<>
std::string Debug::Detail::ToString(const bool& value, uint32_t level) {
    std::stringstream stream;
    stream << (value ? "true" : "false");
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const RenderingSession& session, uint32_t level) {
    std::stringstream stream;
    stream
    << "RenderingSession {" << std::endl
    << Debug::indent(level + 1) << "Resolution: " << session.getResolution() << std::endl
    << Debug::indent(level + 1) << "NbThreads: " << session.getNbThreads() << std::endl
    << Debug::indent(level + 1) << "UseBVH: "
    << Debug::Detail::ToString(session.useBVH(), level + 1) << std::endl
    << Debug::indent(level + 1) << "RenderedSamples: " << session.getRenderedSamples() << std::endl
    << Debug::indent(level + 1) << "Scene: "
    << Debug::Detail::ToStringPtr(session.getScene(), level + 1) << std::endl
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Renderer& renderer, uint32_t level) {
    std::stringstream stream;
    stream << "Renderer {" << std::endl
    << Debug::indent(level + 1) << "RenderingSession: "
    << Debug::Detail::ToStringPtr(renderer.getRenderingSession(), level + 1) << std::endl
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Scene& scene, uint32_t level) {
    std::stringstream stream;
    stream << "Scene {" << std::endl
    << Debug::indent(level + 1) << "Exposure: " << scene.getExposure() << std::endl
    << Debug::indent(level + 1) << "BackgroundColor: " << scene.getBackgroundColor() << std::endl
    << Debug::indent(level + 1) << "RootNode: " << ToStringPtr(scene.getRootNode(), level + 1)
    << std::endl
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Material& material, uint32_t level) {
    std::stringstream stream;
    stream << "Material {" << std::endl
    << Debug::indent(level + 1) << "Color: " << material.getColor() << std::endl
    << Debug::indent(level + 1) << "Diffuse: " << material.getDiffuse() << std::endl
    << Debug::indent(level + 1) << "Specular: " << material.getSpecular() << std::endl
    << Debug::indent(level + 1) << "Shininess: " << material.getShininess() << std::endl
    << Debug::indent(level + 1) << "Reflection: " << material.getReflection() << std::endl
    << Debug::indent(level + 1) << "Refraction: " << material.getRefraction() << std::endl
    << Debug::indent(level + 1) << "Glossiness: " << material.getGlossiness() << std::endl
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}


template<>
std::string Debug::Detail::ToString(const Node& node, uint32_t level) {
    std::stringstream stream;
    stream << "Node {" << std::endl
    << Debug::indent(level + 1) << "Name: \"" << node.getName() << "\"" << std::endl
    << Debug::indent(level + 1) << "Position: " << node.getPosition() << std::endl
    << Debug::Detail::ToStringNodeChilds(node, level + 1)
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

std::string Debug::Detail::ToStringNodeChilds(const Node& node, uint32_t level) {
    std::stringstream stream;
    const std::vector<Node*>& childs = node.getChilds();
    if (childs.size() > 10) {
        stream << Debug::indent(level) << "Childs: " << childs.size() << std::endl;
    } else {
        stream << Debug::indent(level) << "Childs: {" << std::endl;
        for (auto it = childs.begin(), end = childs.end(); it != end; ++it) {
            stream << Debug::indent(level + 1);
            
            Node* child = *it;
            // Try to dynamic cast the node into different node types:
            if (dynamic_cast<const Sphere*>(child)) {
                stream << Debug::Detail::ToStringPtr(dynamic_cast<const Sphere*>(child), level + 2)
                << std::endl;
            } else if (dynamic_cast<const Plane*>(child)) {
                stream << Debug::Detail::ToStringPtr(dynamic_cast<const Plane*>(child), level + 2)
                << std::endl;
            } else if (dynamic_cast<const Camera*>(child)) {
                stream << Debug::Detail::ToStringPtr(dynamic_cast<const Camera*>(child), level + 2)
                << std::endl;
            } else if (dynamic_cast<const Object*>(child)) {
                stream << Debug::Detail::ToStringPtr(dynamic_cast<const Object*>(child), level + 2)
                << std::endl;
            } else {
                stream << Debug::Detail::ToStringPtr(child, level + 2) << std::endl;
            }
        }
        stream << Debug::indent(level) << "}" << std::endl;
    }
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Camera& node, uint32_t level) {
    std::stringstream stream;
    stream << "Camera {" << std::endl
    << Debug::indent(level + 1) << "Name: \"" << node.getName() << "\"" << std::endl
    << Debug::indent(level + 1) << "Size: " << node.getSize() << std::endl
    << Debug::indent(level + 1) << "Position: " << node.getPosition() << std::endl
    << Debug::indent(level + 1) << "Target: " << node.getTarget() << std::endl
    << Debug::indent(level + 1) << "Fovy: " << node.getFovy() << std::endl
    << Debug::indent(level + 1) << "Up vector: " << node.getUpVector() << std::endl
    << Debug::indent(level + 1) << "Near clipping plane: " << node.getNearClippingPlane()
    << std::endl
    << Debug::indent(level + 1) << "Aperture: " << node.getAperture() << std::endl
    << Debug::indent(level + 1) << "Focus distance: " << node.getFocusDistance() << std::endl
    << Debug::Detail::ToStringNodeChilds(node, level + 1)
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Light& node, uint32_t level) {
    std::stringstream stream;
    stream << "Light {" << std::endl
    << Debug::indent(level + 1) << "Name: \"" << node.getName() << "\"" << std::endl
    << Debug::indent(level + 1) << "Position: " << node.getPosition() << std::endl
    << Debug::indent(level + 1) << "Color: " << node.getColor() << std::endl
    << Debug::indent(level + 1) << "Radius: " << node.getRadius() << std::endl
    << Debug::Detail::ToStringNodeChilds(node, level + 1)
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Object& node, uint32_t level) {
    std::stringstream stream;
    stream << "Object {" << std::endl
    << Debug::indent(level + 1) << "Name: \"" << node.getName() << "\"" << std::endl    
    << Debug::indent(level + 1) << "Position: " << node.getPosition() << std::endl
    << Debug::indent(level + 1) << "Material: " << node.getMaterial() << std::endl
    << Debug::Detail::ToStringNodeChilds(node, level + 1)
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Sphere& node, uint32_t level) {
    std::stringstream stream;
    stream << "Sphere {" << std::endl
    << Debug::indent(level + 1) << "Name: \"" << node.getName() << "\"" << std::endl    
    << Debug::indent(level + 1) << "Position: " << node.getPosition() << std::endl
    << Debug::indent(level + 1) << "Material: " << ToStringPtr(node.getMaterial(), level + 1)
    << std::endl
    << Debug::indent(level + 1) << "Radius: " << node.getRadius() << std::endl
    << Debug::Detail::ToStringNodeChilds(node, level + 1)
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

template<>
std::string Debug::Detail::ToString(const Plane& node, uint32_t level) {
    std::stringstream stream;
    stream << "Plane {" << std::endl
    << Debug::indent(level + 1) << "Name: \"" << node.getName() << "\"" << std::endl
    << Debug::indent(level + 1) << "Position: " << node.getPosition() << std::endl
    << Debug::indent(level + 1) << "Material: " << ToStringPtr(node.getMaterial(), level + 1)
    << std::endl
    << Debug::indent(level + 1) << "Normal: " << node.getNormal() << std::endl
    << Debug::Detail::ToStringNodeChilds(node, level + 1)
    << Debug::indent(level) << "}"
    ;
    return stream.str();
}

std::string Debug::TimeToString(float time, bool msPrecision) {
    std::stringstream stream;
    if (time < 1) {
        stream << std::setprecision(3) << (time * 1000) << "ms";
    } else {
        int t = (int)time;
        int seconds = t % 60;
        int minutes = (t / 60) % 60;
        int hours = (t / 3600) % 24;
        int days = t / (3600*24);
        bool start = true;
        if (days > 0) {
            stream << days << "d";
            start = false;
        }
        if (hours > 0 || !start) {
            if (!start)
                stream << " ";
            start = false;
            stream << hours << "h";
        }
        if (minutes > 0 || !start) {
            if (!start)
                stream << " ";
            start = false;
            stream << minutes << "m";
        }
        if (!start)
            stream << " ";
        stream << seconds << "s";
        if (msPrecision) {
            stream << " " << (int)((time - (int)time) * 1000) << "ms";
        }
    }
    return stream.str();
}