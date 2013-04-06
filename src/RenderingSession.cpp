//
//  RenderingSession.cpp
//  SimplePathTracer
//
//  Created by gael on 31/03/13.
//
//

#include "RenderingSession.h"

#include <iomanip>

#include "Debug.h"

RenderingSession::RenderingSession(void) :
    _scene(NULL), _renderer(NULL), _resolution(), _nbSamples(-1),
    _nbThreads(1), _useBVH(true), _bvhMaxDepth(20),
    _renderBuffer(), _renderedSamples(0), _totalEllapsedTime(0), _avgTimePerSample(-1)
{
    
}

RenderingSession::~RenderingSession(void) {
    
}

void RenderingSession::setScene(Scene* scene) {
    _scene = scene;
}

Scene* RenderingSession::getScene(void) const {
    return _scene;
}

void RenderingSession::setRenderer(Renderer *renderer) {
    _renderer = renderer;
    _renderer->setRenderingSession(this);
}

Renderer* RenderingSession::getRenderer(void) const {
    return _renderer;
}

void RenderingSession::setResolution(const vec2& resolution) {
    _resolution = resolution;
}

vec2 RenderingSession::getResolution(void) const {
    return _resolution;
}

void RenderingSession::setNbSamples(int32_t nbSamples) {
    _nbSamples = nbSamples;
}

int32_t RenderingSession::getNbSamples(void) const {
    return _nbSamples;
}

void RenderingSession::setNbThreads(uint32_t nbThreads) {
    _nbThreads = nbThreads;
}

uint32_t RenderingSession::getNbThreads(void) const {
    return _nbThreads;
}

void RenderingSession::setUseBVH(bool useBVH) {
    _useBVH = useBVH;
}

bool RenderingSession::useBVH(void) const {
    return _useBVH;
}

void RenderingSession::setBVHMaxDepth(uint32_t maxDepth) {
    _bvhMaxDepth = maxDepth;
}

uint32_t RenderingSession::getBVHMaxDepth(void) const {
    return _bvhMaxDepth;
}

const Image& RenderingSession::getRenderBuffer(void) const {
    return _renderBuffer;
}

Image& RenderingSession::getRenderBuffer(void) {
    return _renderBuffer;
}

void RenderingSession::setRenderedSamples(uint32_t renderedSamples) {
    _renderedSamples = renderedSamples;
}

uint32_t RenderingSession::getRenderedSamples(void) const {
    return _renderedSamples;
}

void RenderingSession::setLastSampleTime(float time) {
    if (_avgTimePerSample < 0.0f)
        _avgTimePerSample = time;
    else
        _avgTimePerSample = (_avgTimePerSample * (_renderedSamples - 1) + time) / _renderedSamples;
}

void RenderingSession::logRenderingTime(float time) {
    _totalEllapsedTime += time;
}

void RenderingSession::setTotalEllapsedTime(float time) {
    _totalEllapsedTime = time;
}

std::string RenderingSession::getStats(void) {
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

void RenderingSession::render(void) {
    if (!_renderer || !_scene) {
        return ;
    }
    
    // Create the render buffer
    _renderBuffer.setSize(_resolution);
    
    _renderer->render();
}