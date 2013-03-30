//
//  Scene.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Scene.h"

#include <iostream>
#include <algorithm>

Scene::Scene(void) :
    _rootNode(new Node()), _nodes(), _lights(), _currentCamera(NULL), _exposure(2.0), _backgroundColor()
{
}

Scene::~Scene(void) {
}

void Scene::update(void) {
    // Sort nodes in a list, and extract lights in a separate one
    _nodes.clear();
    _lights.clear();
    _addNodeToLists(_rootNode);
    
    // Update scene graph
    _rootNode->update();
}

void Scene::setExposure(float exposure) {
    _exposure = exposure;
}

float Scene::getExposure(void) const {
    return _exposure;
}

void Scene::setBackgroundColor(const vec3& color) {
    _backgroundColor = color;
}

vec3 Scene::getBackgroundColor(void) const {
    return _backgroundColor;
}

void Scene::addNode(Node *node) {
    _rootNode->addChild(node);
    Camera* camera = dynamic_cast<Camera*>(node);
    if (camera)
        _currentCamera = camera;
}

void Scene::removeNode(Node* node) {
    if (node == _currentCamera)
        _currentCamera = NULL;
}

Scene& Scene::operator<<(Node* node) {
    addNode(node);
    return *this;
}

Camera* Scene::getCurrentCamera(void) {
    return _currentCamera;
}

Node* Scene::getRootNode(void) {
    return _rootNode;
}

std::vector<Node*>& Scene::getNodes(void) {
    return _nodes;
}

std::vector<Light*>& Scene::getLights(void) {
    return _lights;
}

void Scene::_addNodeToLists(Node* node) {
    _nodes.push_back(node);
    Light* light = dynamic_cast<Light*>(node);
    if (light) {
        _lights.push_back(light);
    }
    std::vector<Node*>& childs = node->getChilds();
    for (auto it = childs.begin(), end = childs.end(); it != end; ++it) {
        _addNodeToLists(*it);
    }
}