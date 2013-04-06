//
//  Node.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Node.h"

#include <algorithm>
#include <boost/regex.hpp>
#include <glm/gtc/matrix_transform.hpp>

Node::Node(std::string name) :
    Node(vec3())
{
    _name = name;
}

Node::Node(vec3 position) :
    _name(""), _parent(NULL), _childs(), _position(position), _scale(1),
    _rotationMatrix(), _transformationMatrix(), _absolutePosition(),
    _boundingBox(), _objectId(0)
{
}

Node::~Node(void) {
}

void Node::update(void) {    
    if (_parent) {
        _transformationMatrix = _parent->getTransformationMatrix();
    }    
    else
        _transformationMatrix = mat4();
    
    _transformationMatrix = translate(_transformationMatrix, _position);
    _transformationMatrix *= _rotationMatrix;
    _transformationMatrix = scale(_transformationMatrix, _scale);

    _absolutePosition = vec3();
    _absolutePosition = vec3(_transformationMatrix * vec4(_absolutePosition, 1));
    
    // Recursively update every child
    for (auto it = _childs.begin(), end = _childs.end(); it != end; ++it) {
        (*it)->update();
    }
}

std::string Node::getName(void) const {
    return _name;
}

void Node::setName(const std::string name) {
    _name = name;
}

void Node::setPosition(const vec3 position) {
    _position = position;
}

vec3 Node::getPosition(void) const {
    return _position;
}

void Node::setScale(const vec3 scale) {
    _scale = scale;
}

vec3 Node::getScale(void) const {
    return _scale;
}

void Node::setRotation(float angle, const vec3& axis) {
    _rotationMatrix = rotate(_rotationMatrix, angle, axis);
}

const mat4& Node::getTransformationMatrix(void) {
    return _transformationMatrix;
}

vec3 Node::getAbsolutePosition(void) const {
    return _absolutePosition;
}

float Node::intersectWithRay(const Ray&) {
    return -1.0f;
}

bool Node::hasBoundingBox(void) const {
    return false;
}

const BoundingBox& Node::getBoundingBox(void) const {
    return _boundingBox;
}

void Node::setParent(Node *node) {
    _parent = node;
}

Node* Node::getParent(void) {
    return _parent;
}

void Node::addChild(Node *node) {
    addChild(node, false);
}

void Node::addChild(Node *node, bool fastInsert) {
    if (fastInsert || std::find(_childs.begin(), _childs.end(), node) == _childs.end()) {
        _childs.push_back(node);
        node->setParent(this);
    }
}

std::vector<Node*>& Node::getChilds(void) {
    return _childs;
}

const std::vector<Node*>& Node::getChilds(void) const {
    return _childs;
}

void Node::removeChild(Node* node) {
    auto it = std::find(_childs.begin(), _childs.end(), node);
    if (it != _childs.end()) {
        _childs.erase(it);
    }
}

void Node::remove(void) {
    if (_parent) {
        _parent->removeChild(this);
    }
}

static bool compareNodes(Node* n1, Node* n2) {
    return n1->getName() < n2->getName();
}

Node::List Node::search(const std::string& regex) {
    Node::List result;
    
    boost::regex ex(regex);
    if (!_name.empty() && boost::regex_match(_name, ex)) {
        result.push_back(this);
    }
    for (auto it = _childs.begin(), end = _childs.end(); it != end; ++it) {
        Node::List childs = (*it)->search(regex);
        if (childs.size() > 0) {
            result.insert(result.end(), childs.begin(), childs.end());
        }
    }
    std::sort(result.begin(), result.end(), compareNodes);
    return result;
}

void Node::setObjectId(uint32_t id) {
    _objectId = id;
}

uint32_t Node::getObjectId(void) const {
    return _objectId;
}