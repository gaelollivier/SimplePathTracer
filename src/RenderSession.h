//
//  RenderSession.h
//  SimplePathTracer
//
//  Created by gael on 31/03/13.
//
//

#ifndef __SimplePathTracer__RenderSession__
#define __SimplePathTracer__RenderSession__

#include <string>
#include <glm/glm.hpp>

#include "Scene.h"

using namespace glm;

class RenderSession {
public:
    RenderSession(void);
    ~RenderSession(void);
    
    void setResolution(const vec2& resolution);
    vec2 getResolution(void) const;
    
    void        setNbThreads(uint32_t nbThreads);
    uint32_t    getNbThreads(void) const;
    
    void    setScene(Scene* scene);
    Scene*  getScene(void) const;
    
    void setUseBVH(bool useBVH);
    bool useBVH(void) const;
    
    void        setBVHMaxDepth(uint32_t maxDepth);
    uint32_t    getBVHMaxDepth(void) const;
    
    void    setNbSamples(int32_t nbSamples);
    int32_t getNbSamples(void) const;
    
    void        setRenderedSamples(uint32_t renderedSamples);
    uint32_t    getRenderedSamples(void) const;
    
    void setLastSampleTime(float time);
    void logRenderingTime(float time);    
    void setTotalEllapsedTime(float time);
    
    std::string getStats(void);
    
private:
    vec2        _resolution;
    uint32_t    _nbThreads;
    Scene*      _scene;
    bool        _useBVH;
    uint32_t    _bvhMaxDepth;
    int32_t     _nbSamples;
    uint32_t    _renderedSamples;
    float       _totalEllapsedTime;
    float       _avgTimePerSample;
};

#endif /* defined(__SimplePathTracer__RenderSession__) */
