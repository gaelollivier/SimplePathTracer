//
//  RenderingSession.h
//  SimplePathTracer
//
//  Created by gael on 31/03/13.
//
//

#ifndef __SimplePathTracer__RenderingSession__
#define __SimplePathTracer__RenderingSession__

#include <string>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Image.h"
#include "Renderer.h"

using namespace glm;

class RenderingSession {
public:
    
    RenderingSession(void);
    ~RenderingSession(void);
    
    void    setScene(Scene* scene);
    Scene*  getScene(void) const;
    
    void        setRenderer(Renderer* renderer);
    Renderer*   getRenderer(void) const;
    
    void setResolution(const vec2& resolution);
    vec2 getResolution(void) const;
    
    void    setNbSamples(int32_t nbSamples);
    int32_t getNbSamples(void) const;
    
    void        setNbThreads(uint32_t nbThreads);
    uint32_t    getNbThreads(void) const;
    
    void setUseBVH(bool useBVH);
    bool useBVH(void) const;
    
    void        setBVHMaxDepth(uint32_t maxDepth);
    uint32_t    getBVHMaxDepth(void) const;
    
    const Image& getRenderBuffer(void) const;
    Image&       getRenderBuffer(void);
    
    void        setRenderedSamples(uint32_t renderedSamples);
    uint32_t    getRenderedSamples(void) const;
    
    void setLastSampleTime(float time);
    void logRenderingTime(float time);    
    void setTotalEllapsedTime(float time);
    
    std::string getStats(void);
    
    void render(void);
    
private:
    Scene*      _scene;
    Renderer*   _renderer;
    vec2        _resolution;
    int32_t     _nbSamples;
    uint32_t    _nbThreads;
    bool        _useBVH;
    uint32_t    _bvhMaxDepth;
    Image       _renderBuffer;
    uint32_t    _renderedSamples;
    float       _totalEllapsedTime;
    float       _avgTimePerSample;
};

#endif /* defined(__SimplePathTracer__RenderingSession__) */
