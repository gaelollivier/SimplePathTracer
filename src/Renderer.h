//
//  Renderer.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Renderer__
#define __SimplePathTracer__Renderer__

#include <vector>
#include <SFML/System.hpp>

#include "Ray.h"
#include "Scene.h"
#include "KdTree.h"

class Renderer {
public:
    
    static const float      Epsilon;
    static const uint32_t   PathLength;
    
    struct ThreadContext {
        ThreadContext(Renderer* renderer, uint16_t threadID);
        
        Renderer*   renderer;
        uint16_t    threadID;
    };
    
    Renderer(vec2 const& imageSize, uint16_t nbThreads=8);
    ~Renderer(void);
    
    vec2 getImageSize(void) const;
    vec3* getResultBuffer(void) const;
    
    void setScene(Scene* scene);
    
    void buildKdTree(void);
    
    void render(void);
    
    void renderSection(uint16_t threadID);
    
    vec3 renderPixel(float x, float y);
    
    vec3    traceRay(const Ray& ray, uint32_t level=0);
    float   intersectWithNodes(const Ray& ray, Node*& intersectedNode);
    bool    castShadowRay(const Ray& ray, float maxDist);
    vec3    getDirectLightning(const Ray& ray, const vec3& intersectPoint,
                               const vec3& normal, Object* intersectedObject);
    Ray    getBRDFRay(const Ray& ray, const vec3& intersectPoint,
                      const vec3& normal, Object* intersectedObject);
    
    // Random vector generators
    static vec3 uniformSample(void);
    static vec3 cosineSampleHemisphere(const vec3& normal);
    
    static void renderFunction(ThreadContext context);
    
private:
    uint16_t                    _nbThreads;
    std::vector<sf::Thread*>    _threads;
    
    vec2                        _imageSize;
    vec3*                       _result;
    Scene*                      _scene;
    KdTree*                     _kdTree;
};


#endif /* defined(__SimplePathTracer__Renderer__) */
