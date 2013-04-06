//
//  Renderer.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Renderer.h"

#include <iostream>
#include <algorithm>
#include <boost/bind.hpp>
#include <SFML/System.hpp>

#include "RenderingSession.h"

const float     Renderer::Epsilon = 0.0001;
const uint32_t  Renderer::PathLength = 5;

Renderer::ThreadContext::ThreadContext(Renderer* renderer, uint16_t threadID) :
    renderer(renderer), threadID(threadID)
{
}

Renderer::Renderer(void) :
    _renderingSession(NULL), _threads(),
    _kdTree(NULL)
{
}

Renderer::~Renderer(void) {
    for (uint16_t thread = 0; thread < _threads.size(); ++thread) {
        if (_threads[thread]) {
            _threads[thread]->interrupt();
            delete _threads[thread];
        }
    }
    if (_kdTree)
        delete _kdTree;
}

void Renderer::setRenderingSession(RenderingSession *renderingSession) {
    _renderingSession = renderingSession;
}

RenderingSession* Renderer::getRenderingSession(void) const {
    return _renderingSession;
}

void Renderer::buildBVH(void) {
    if (!_renderingSession) {
        std::cerr << "Renderer: No Render Session specified" << std::endl;
        return ;
    }
    if (!_renderingSession->getScene()) {
        std::cerr << "Renderer: No Scene specified" << std::endl;
        return ;
    }
    if (_kdTree)
        delete _kdTree;
    sf::Clock timer;
    _kdTree = new KdTree();
    _kdTree->setMaxDepth(_renderingSession->getBVHMaxDepth());
    _kdTree->build(_renderingSession->getScene()->getNodes());
    _renderingSession->logRenderingTime(timer.getElapsedTime().asSeconds());
}

void Renderer::render(void) {
    if (_renderingSession->useBVH() && !_kdTree)
        buildBVH();
    
    sf::Clock timer;
    
    // Create the rendering threads
    _threads.resize(_renderingSession->getNbThreads());
    
    // Launch the rendering threads
    for (uint16_t thread = 0; thread < _threads.size(); ++thread) {
        ThreadContext context(this, thread);
        _threads[thread] = new boost::thread(boost::bind(&Renderer::renderFunction, context));
    }
    
    // Wait for threads
    for (uint16_t thread = 0; thread < _threads.size(); ++thread) {
        if (_threads[thread]) {
            _threads[thread]->join();
            delete _threads[thread];
        }
    }
    
    _threads.clear();
    
    // Increment the number of rendered samples
    _renderingSession->setRenderedSamples(_renderingSession->getRenderedSamples() + 1);
    
    _renderingSession->setLastSampleTime(timer.getElapsedTime().asSeconds());
    _renderingSession->logRenderingTime(timer.getElapsedTime().asSeconds());
}

void Renderer::renderFunction(ThreadContext context) {
    context.renderer->renderSection(context.threadID);
}

void Renderer::renderSection(uint16_t threadID) {
    vec2 resolution = _renderingSession->getResolution();
    float xStart = ((float)threadID / _threads.size()) * resolution.x;
    float xEnd = xStart + (resolution.x / _threads.size());
    for (float x = xStart; x < xEnd; ++x) {
        for (float y = 0; y < resolution.y; ++y) {
            vec2 pixel = vec2(x, y);
            vec3 value = _renderingSession->getRenderBuffer().getPixel(pixel);
            value += renderPixel(x, y);
            _renderingSession->getRenderBuffer().setPixel(pixel, value);
        }
    }
}

vec3 Renderer::renderPixel(float x, float y) {
    vec3 finalColor = vec3(0, 0, 0);
    
    vec2 resolution = _renderingSession->getResolution();
    
    // Normalized pixel coordinates, y axis pointing upwards
    vec2 point = vec2(x / resolution.x, 1.0 - (y / resolution.y));
    vec2 sample = vec2((float)random() / RAND_MAX,
                        (float)random() / RAND_MAX);
    point += sample / resolution;
    // Calc initial ray
    Camera* camera = _renderingSession->getScene()->getCurrentCamera();
    Ray ray = camera->getRayAtPoint(point);
    
    finalColor += traceRay(ray);
    
    finalColor = vec3(1, 1, 1) - exp(finalColor * - _renderingSession->getScene()->getExposure());
    
    return finalColor;
}

vec3 Renderer::traceRay(const Ray& ray, uint32_t level) {
    if (level > PathLength)
        return vec3(0, 0, 0);
    Node* intersectedNode = NULL;
    
    float d;
    if (_kdTree)
        d = _kdTree->traceRay(ray, intersectedNode);
    else
        d = intersectWithNodes(ray, intersectedNode);
    
    if (d > Epsilon) {
        Object* intersectedObject = dynamic_cast<Object*>(intersectedNode);
        if (intersectedObject) {
            vec3 intersectPoint = ray.origin + ray.direction * d;
            vec3 normal = intersectedObject->normalAtPoint(intersectPoint);
            vec3 surfaceColor = intersectedObject->getMaterial()->getColor();
            vec3 directLightning = getDirectLightning(ray, intersectPoint, normal, intersectedObject);
            Ray BRDFRay = getBRDFRay(ray, intersectPoint, normal, intersectedObject);
            vec3 indirectLightning = traceRay(BRDFRay, level + 1);
            return surfaceColor * indirectLightning + directLightning;
        }
        Light* intersectedLight = dynamic_cast<Light*>(intersectedNode);
        if (intersectedLight) {
            return intersectedLight->getColor();
        }
    }
    // No object hit, return background color
    return _renderingSession->getScene()->getBackgroundColor();
}

float Renderer::intersectWithNodes(const Ray& ray, Node*& intersectedNode) {
    std::vector<Node*>& nodes = _renderingSession->getScene()->getNodes();
    Node* node = NULL;
    float minD = -1.0;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        float d = (*it)->intersectWithRay(ray);
        if (d > Epsilon && (node == NULL || d < minD)) {
            minD = d;
            node = *it;
        }
    }
    if (node)
        intersectedNode = node;
    return minD;
}

bool Renderer::castShadowRay(const Ray& ray, float maxDist) {
    std::vector<Node*>& nodes = _renderingSession->getScene()->getNodes();
    for (auto it = nodes.begin(), end = nodes.end(); it != end; ++it) {
        if (!dynamic_cast<Object*>(*it))
            continue;
        float d = (*it)->intersectWithRay(ray);
        if (d > Epsilon && d < maxDist) {
            return true;
        }
    }
    return false;
}

vec3 Renderer::getDirectLightning(const Ray& ray, const vec3& intersectPoint,
                                  const vec3& normal, Object* intersectedObject) {
    vec3 directLight = vec3(0, 0, 0);
    // If the object reflect, don't compute direct light
    if (intersectedObject->getMaterial()->getDiffuse() <= 0.0
        && intersectedObject->getMaterial()->getSpecular() <= 0.0)
        return directLight;
    const std::vector<Light*>& lights = _renderingSession->getScene()->getLights();
    for (auto it = lights.begin(), end = lights.end(); it != end; ++it) {
        Light* light = *it;
        // Cast shadow ray
        vec3 toLight = vec3(light->getSampledPosition() - intersectPoint);
        Ray lightRay = Ray(intersectPoint, normalize(toLight));
        bool shadow;
        if (_kdTree)
            shadow = _kdTree->castShadow(lightRay, length(toLight));
        else
            shadow = castShadowRay(lightRay, length(toLight));
        if (!shadow) {
            // Diffuse light
            float diffuseCoeff = std::max(0.0f, dot(normal, lightRay.direction));
            directLight += intersectedObject->getMaterial()->getColor() * light->getColor()
                            * diffuseCoeff * intersectedObject->getMaterial()->getDiffuse();
            
            // Specular light
    		vec3 reflected = normalize(reflect(lightRay.direction, normal));
    		float specCosAlpha = dot(normalize(ray.direction), reflected);
    		float specCoeff = pow(specCosAlpha,
                                  intersectedObject->getMaterial()->getShininess()) * diffuseCoeff;
    		if (specCoeff > 0.0) {
	    		directLight += light->getColor() * specCoeff
                                * intersectedObject->getMaterial()->getSpecular();
	    	}
        }
    }
    return directLight;
}

Ray Renderer::getBRDFRay(const Ray& ray, const vec3& intersectPoint,
                          const vec3& normal, Object* intersectedObject) {
    if (intersectedObject->getMaterial()->getReflection() > 0.0) {
        return Ray(intersectPoint, normalize(reflect(ray.direction, normal) + cosineSampleHemisphere(normal) * intersectedObject->getMaterial()->getGlossiness()));
    } else if (intersectedObject->getMaterial()->getRefraction() > 0.0) {
        Ray newRay;
        float n1 = 1.0;
        float n2 = 1.0;
        if (ray.objectId != intersectedObject->getObjectId() || 1) {
            newRay = Ray(intersectPoint, normalize(refract(ray.direction, normal, 0.3)));
        } else {
            newRay = Ray(intersectPoint, normalize(refract(ray.direction, normal, n2/n1)));
        }
        newRay.objectId = intersectedObject->getObjectId();
        return newRay;
    } else {
        return Ray(intersectPoint, normalize(cosineSampleHemisphere(normal)));
    }
}

vec3 Renderer::cosineSampleHemisphere(const vec3& normal) {
    // From http://madebyevan.com/webgl-path-tracing/webgl-path-tracing.js
    float u = (float)random() / RAND_MAX;
    float v = (float)random() / RAND_MAX;
    float r = sqrt(u);
    float angle = 6.283185307179586 * v;
    // compute basis from normal
    vec3 sdir, tdir;
    if (abs(normal.x)<.5) {
        sdir = cross(normal, vec3(1,0,0));
    } else {
        sdir = cross(normal, vec3(0,1,0));
    }
    tdir = cross(normal, sdir);
    return r*cos(angle)*sdir + r*sin(angle)*tdir + (float)sqrt(1.-u) * normal;
}

vec3 Renderer::uniformSample(void) {
    return normalize(vec3((float)random() / RAND_MAX,
                          (float)random() / RAND_MAX,
                          (float)random() / RAND_MAX));
}