//
//  Module.hpp
//  SimplePathTracer
//
//  Created by gael on 31/03/13.
//
//

#ifndef SimplePathTracer_Module_hpp
#define SimplePathTracer_Module_hpp

#define PYTHON_MODULE_NAME PySpt
#define PYTHON_MODULE_PYTHON_NAME "PySpt"
#define PYTHON_MODULE_INIT initPySpt

#include <boost/python.hpp>
#include <glm/glm.hpp>

using namespace glm;

#include "Extensions.hpp"
#include "Image.h"
#include "Gui.h"
#include "RenderSession.h"
#include "Renderer.h"
#include "Scene.h"
#include "Material.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

BOOST_PYTHON_MODULE(PYTHON_MODULE_NAME)
{
    
    //
    // Vec2
    //
    boost::python::class_<vec2>("vec2")
    
    // Constructors
    .def(boost::python::init<float>())
    .def(boost::python::init<float, float>())
    
    // Operators
    .def("__str__", &Debug::ToString<vec2>)
    .def("__div__", &Extensions::Operators::Div<vec2, float>)
    .def("__div__", &Extensions::Operators::Div<vec2, vec2>)
    .def("__mult__", &Extensions::Operators::Mult<vec2, float>)
    .def("__mult__", &Extensions::Operators::Mult<vec2, vec2>)
    .def("__add__", &Extensions::Operators::Add<vec2, float>)
    .def("__add__", &Extensions::Operators::Add<vec2, vec2>)
    .def("__sub__", &Extensions::Operators::Sub<vec2, float>)
    .def("__sub__", &Extensions::Operators::Sub<vec2, vec2>)
    
    // Members
    .def_readwrite("x", &vec2::x)
    .def_readwrite("y", &vec2::y)
    ;

    //
    // Vec3
    //
    boost::python::class_<vec3>("vec3")
    
    // Constructors
    .def(boost::python::init<float>())
    .def(boost::python::init<float, float, float>())
    
    // Operators
    .def("__str__", &Debug::ToString<vec3>)
    .def("__div__", &Extensions::Operators::Div<vec3, float>)
    .def("__div__", &Extensions::Operators::Div<vec3, vec3>)
    .def("__mult__", &Extensions::Operators::Mult<vec3, float>)
    .def("__mult__", &Extensions::Operators::Mult<vec3, vec3>)
    .def("__add__", &Extensions::Operators::Add<vec3, float>)
    .def("__add__", &Extensions::Operators::Add<vec3, vec3>)
    .def("__sub__", &Extensions::Operators::Sub<vec3, float>)
    .def("__sub__", &Extensions::Operators::Sub<vec3, vec3>)
    
    // Members
    .def_readwrite("x", &vec3::x)
    .def_readwrite("y", &vec3::y)
    .def_readwrite("z", &vec3::z)

    .def_readwrite("r", &vec3::r)
    .def_readwrite("g", &vec3::g)
    .def_readwrite("b", &vec3::b)
    ;
    
    //
    // Image
    //
    boost::python::class_<Image>("Image")
    // Get/Set
    .def("clear", &Image::clear)
    .def("setSize", &Image::setSize)
    .def("getSize", &Image::getSize)
    .def("getPixel", &Image::getPixel)
    .def("setPixel", &Image::setPixel)
    ;
    
    //
    // GUI
    //
    boost::python::class_<Gui, boost::noncopyable>("Gui", boost::python::no_init)
    .def("show", &Gui::show)
    .staticmethod("show")
    .def("saveImage", &Gui::saveImage)
    .staticmethod("saveImage")
    ;
    
    //
    // RenderSession
    //    
    boost::python::class_<RenderSession>("RenderSession")
    // Operators
    .def("__str__", &Debug::ToString<RenderSession>)
    
    // Get/Set
    .def("setResolution", &RenderSession::setResolution)
    .def("getResolution", &RenderSession::getResolution)
    .def("setNbThreads", &RenderSession::setNbThreads)
    .def("getNbThreads", &RenderSession::getNbThreads)
    .def("setScene", &RenderSession::setScene)
    .def("getScene", &RenderSession::getScene,
         boost::python::return_value_policy<boost::python::reference_existing_object>())
    .def("setUseBVH", &RenderSession::setUseBVH)
    .def("setBVHMaxDepth", &RenderSession::setBVHMaxDepth)
    .def("getBVHMaxDepth", &RenderSession::getBVHMaxDepth)
    .def("useBVH", &RenderSession::useBVH)
    .def("setNbSamples", &RenderSession::setNbSamples)
    .def("getNbSamples", &RenderSession::getNbSamples)
    .def("setRenderedSamples", &RenderSession::setRenderedSamples)
    .def("getRenderedSamples", &RenderSession::getRenderedSamples)
    
    // Stats functions
    .def("setLastSampleTime", &RenderSession::setLastSampleTime)
    .def("logRenderingTime", &RenderSession::logRenderingTime)
    .def("setTotalEllapsedTime", &RenderSession::setTotalEllapsedTime)
    .def("getStats", &RenderSession::getStats)
    ;
    
    //
    // Renderer
    //
    boost::python::class_<Renderer>("Renderer")
    // Operators
    .def("__str__", &Debug::ToString<Renderer>)
    
    // Get/Set
    .def("setRenderSession", &Renderer::setRenderSession)
    .def("getRenderSession", &Renderer::getRenderSession,
         boost::python::return_value_policy<boost::python::reference_existing_object>())
    
    // Member functions
    .def("buildBVH", &Renderer::buildBVH)
    .def("render", &Renderer::render)
    .def("getRenderBuffer", &Renderer::getRenderBuffer,
         boost::python::return_value_policy<boost::python::reference_existing_object>())
    ;
    
    //
    // Scene
    //
    boost::python::class_<Scene>("Scene")
    // Operators
    .def("__str__", &Debug::ToString<Scene>)
    
    // Member functions
    .def("addNode", &Scene::addNode)
    .def("update", &Scene::update)
    ;
    
    //
    // Material
    //
    boost::python::class_<Material>("Material")
    // Constructors
    .def(boost::python::init<>())
    .def(boost::python::init<vec3>())
    
    // Operators
    .def("__str__", &Debug::ToString<Material>)
    
    // Get/Set
    .def("getColor", &Material::getColor)
    .def("setColor", &Material::setColor)
    .def("getReflection", &Material::getReflection)
    .def("setReflection", &Material::setReflection)
    .def("getRefraction", &Material::getRefraction)
    .def("setRefraction", &Material::setRefraction)
    .def("getDiffuse", &Material::getDiffuse)
    .def("setDiffuse", &Material::setDiffuse)
    .def("getSpecular", &Material::getSpecular)
    .def("setSpecular", &Material::setSpecular)
    .def("getShininess", &Material::getShininess)
    .def("setShininess", &Material::setShininess)
    .def("getGlossiness", &Material::getGlossiness)
    .def("setGlossiness", &Material::setGlossiness)
    ;    
    
    //
    // Node
    //
    
    // Define some function ptr's by hand for overloaded functions
    void (Node::*addChildPtr)(Node*) = &Node::addChild;
    void (Node::*addChildPtr2)(Node*, bool) = &Node::addChild;
    
    boost::python::class_<Node>("Node")
    // Constructors
    .def(boost::python::init<>())
    .def(boost::python::init<std::string>())
    .def(boost::python::init<vec3>())
    
    // Operators
    .def("__str__", &Debug::ToString<Node>)
    
    // Get/Set
    .def("getPosition", &Node::getPosition)
    .def("setPosition", &Node::setPosition)
    .def("addChild", addChildPtr)
    .def("addChild", addChildPtr2)
    
    // Member functions
    .def("update", &Node::update)
    ;
    
    //
    // Camera
    //
    boost::python::class_<Camera, boost::python::bases<Node>>("Camera")
    // Constructors
    .def(boost::python::init<>())
    .def(boost::python::init<vec2, vec3, vec3>())
    
    // Operators
    .def("__str__", &Debug::ToString<Camera>)
    
    // Get/Set
    .def("getSize", &Camera::getSize)
    .def("setSize", &Camera::setSize)
    .def("getTarget", &Camera::getTarget)
    .def("setTarget", &Camera::setTarget)
    .def("getFovy", &Camera::getFovy)
    .def("setFovy", &Camera::setFovy)
    .def("getUpVector", &Camera::getUpVector)
    .def("setUpVector", &Camera::setUpVector)
    .def("getNearClippingPlane", &Camera::getNearClippingPlane)
    .def("setNearClippingPlane", &Camera::setNearClippingPlane)
    .def("getAperture", &Camera::getAperture)
    .def("setAperture", &Camera::setAperture)
    .def("getFocusDistance", &Camera::getFocusDistance)
    .def("setFocusDistance", &Camera::setFocusDistance)
    
    // Member functions
    .def("update", &Camera::update)
    ;

    //
    // Light
    //
    boost::python::class_<Light, boost::python::bases<Node>>("Light")
    // Constructors
    .def(boost::python::init<>())
    .def(boost::python::init<vec3, vec3, float>())
    
    // Operators
    .def("__str__", &Debug::ToString<Object>)
    
    // Member functions
    .def("update", &Light::update)
    .def("getColor", &Light::getColor)
    .def("setColor", &Light::setColor)
    .def("getRadius", &Light::getRadius)
    .def("setRadius", &Light::setRadius)
    ;
    
    //
    // Object
    //
    boost::python::class_<Object, boost::python::bases<Node>>("Object")
    // Constructors
    .def(boost::python::init<>())
    .def(boost::python::init<vec3, Material*>())
    
    // Operators
    .def("__str__", &Debug::ToString<Object>)
    ;
    
    
    //
    // Sphere
    //
    boost::python::class_<Sphere, boost::python::bases<Object>>("Sphere")
    // Constructors
    .def(boost::python::init<vec3, float, Material*>())
    
    // Operators
    .def("__str__", &Debug::ToString<Sphere>)
    
    // Get/Set
    .def("getRadius", &Sphere::getRadius)
    .def("setRadius", &Sphere::setRadius)
    
    // Member functions
    .def("update", &Sphere::update)
    ;
    
    //
    // Plane
    //
    boost::python::class_<Plane, boost::python::bases<Object>>("Plane")
    // Constructors
    .def(boost::python::init<>())
    .def(boost::python::init<vec3, vec3, Material*>())
    
    // Operators
    .def("__str__", &Debug::ToString<Plane>)
    
    // Get/Set
    .def("getNormal", &Plane::getNormal)
    .def("setNormal", &Plane::setNormal)
    
    // Member functions
    .def("update", &Plane::update)
    ;
    
}

#endif