//
//  RenderSession.cpp
//  SimplePathTracer
//
//  Created by gael on 31/03/13.
//
//

#include "RenderSession.h"

#include <iomanip>

#include "Debug.h"

RenderSession::RenderSession(void) :
    _resolution(), _nbThreads(1), _scene(NULL), _useBVH(true), _bvhMaxDepth(20),
    _nbSamples(-1), _renderedSamples(0),
    _totalEllapsedTime(0.0f), _avgTimePerSample(-1.0f)
{
    
}

RenderSession::~RenderSession(void) {
    
}

void RenderSession::setResolution(const vec2& resolution) {
    _resolution = resolution;
}

vec2 RenderSession::getResolution(void) const {
    return _resolution;
}

void RenderSession::setNbThreads(uint32_t nbThreads) {
    _nbThreads = nbThreads;
}

uint32_t RenderSession::getNbThreads(void) const {
    return _nbThreads;
}

void RenderSession::setScene(Scene* scene) {
    _scene = scene;
}

Scene* RenderSession::getScene(void) const {
    return _scene;
}

void RenderSession::setUseBVH(bool useBVH) {
    _useBVH = useBVH;
}

bool RenderSession::useBVH(void) const {
    return _useBVH;
}

void RenderSession::setBVHMaxDepth(uint32_t maxDepth) {
    _bvhMaxDepth = maxDepth;
}

uint32_t RenderSession::getBVHMaxDepth(void) const {
    return _bvhMaxDepth;
}

void RenderSession::setNbSamples(int32_t nbSamples) {
    _nbSamples = nbSamples;
}

int32_t RenderSession::getNbSamples(void) const {
    return _nbSamples;
}

void RenderSession::setRenderedSamples(uint32_t renderedSamples) {
    _renderedSamples = renderedSamples;
}

uint32_t RenderSession::getRenderedSamples(void) const {
    return _renderedSamples;
}

void RenderSession::setLastSampleTime(float time) {
    if (_avgTimePerSample < 0.0f)
        _avgTimePerSample = time;
    else
        _avgTimePerSample = (_avgTimePerSample * (_renderedSamples - 1) + time) / _renderedSamples;
}

void RenderSession::logRenderingTime(float time) {
    _totalEllapsedTime += time;
}

void RenderSession::setTotalEllapsedTime(float time) {
    _totalEllapsedTime = time;
}

std::string RenderSession::getStats(void) {
    std::stringstream stream;
    stream << std::setprecision(3)
    << Debug::TimeToString(_totalEllapsedTime) << " ellapsed, "
    << _renderedSamples;
    if (_nbSamples > 0)
        stream << " / " << _nbSamples;
    stream
    << " samples rendered, "
    << Debug::TimeToString(_avgTimePerSample, true) << " per sample"
    ;
    if (_nbSamples > 0 && _renderedSamples < _nbSamples) {
        stream
        << ", "
        << Debug::TimeToString((_nbSamples - _renderedSamples) * _avgTimePerSample)
        << " left"
        ;
    }
    return stream.str();
}