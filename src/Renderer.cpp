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

const float     Renderer::Epsilon = 0.0001;
const uint32_t  Renderer::PathLength = 3;

Renderer::ThreadContext::ThreadContext(Renderer* renderer, uint16_t threadID) :
    renderer(renderer), threadID(threadID)
{
}

Renderer::Renderer(vec2 const& imageSize, uint16_t nbThreads) :
    _nbThreads(nbThreads), _threads(), _imageSize(imageSize), _result(NULL),
    _scene(NULL), _kdTree(NULL)
{
    // Create the result buffer
    _result = new vec3[(int)_imageSize.x * (int)_imageSize.y];
    
    // Create the threads
    _threads.resize(_nbThreads);
    for (uint16_t thread = 0; thread < _nbThreads; ++thread) {
        ThreadContext context(this, thread);
        _threads[thread] = new sf::Thread(&Renderer::renderFunction, context);
    }
}

Renderer::~Renderer(void) {
    if (_result)
        delete[] _result;
    for (uint16_t thread = 0; thread < _nbThreads; ++thread) {
        if (_threads[thread]) {
            delete _threads[thread];
        }
    }
    if (_kdTree)
        delete _kdTree;
}

vec2 Renderer::getImageSize(void) const {
    return _imageSize;
}

vec3* Renderer::getResultBuffer(void) const {
    return _result;
}

void Renderer::setScene(Scene* scene) {
    _scene = scene;
}

void Renderer::buildKdTree(void) {
    if (_kdTree)
        delete _kdTree;
    _kdTree = new KdTree();
    _kdTree->build(_scene->getNodes());
}

void Renderer::render(void) {
    // Launch the rendering threads
    for (uint16_t thread = 0; thread < _nbThreads; ++thread) {
        if (_threads[thread]) {
            _threads[thread]->launch();
        }
    }
    
    for (uint16_t thread = 0; thread < _nbThreads; ++thread) {
        if (_threads[thread]) {
            _threads[thread]->wait();
        }
    }
}

void Renderer::renderFunction(ThreadContext context) {
    context.renderer->renderSection(context.threadID);
}

void Renderer::renderSection(uint16_t threadID) {
    float xStart = ((float)threadID / _nbThreads) * _imageSize.x;
    float xEnd = xStart + (_imageSize.x / _nbThreads);
    for (float x = xStart; x < xEnd; ++x) {
        for (float y = 0; y < _imageSize.y; ++y) {
            _result[(int)(y * _imageSize.x + x)] += renderPixel(x, y);
        }
    }
}

vec3 Renderer::renderPixel(float x, float y) {
    vec3 finalColor = vec3(0, 0, 0);
    
    // Normalized pixel coordinates, y axis pointing upwards
    vec2 point = vec2(x / _imageSize.x, 1.0 - (y / _imageSize.y));
    vec2 sample = vec2((float)random() / RAND_MAX,
                        (float)random() / RAND_MAX);
    point += sample / _imageSize;
    // Calc initial ray
    Camera* camera = _scene->getCurrentCamera();
    Ray ray = camera->getRayAtPoint(point);
    
    finalColor += traceRay(ray);
    
    finalColor = vec3(1, 1, 1) - exp(finalColor * - _scene->getExposure());
    
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
    return _scene->getBackgroundColor();
}

float Renderer::intersectWithNodes(const Ray& ray, Node*& intersectedNode) {
    std::vector<Node*>& nodes = _scene->getNodes();
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
    std::vector<Node*>& nodes = _scene->getNodes();
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
    std::vector<Light*>& lights = _scene->getLights();
    for (auto it = lights.begin(); it != lights.end(); ++it) {
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