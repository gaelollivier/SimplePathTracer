//
//  Node.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Node.h"

#include <algorithm>

Node::Node(vec3 position) : _parent(NULL), _childs(), _position(position), _boundingBox(), _objectId(0) {
}

Node::~Node(void) {
}

void Node::update(void) {
    _absolutePosition = _position;
    if (_parent)
        _absolutePosition += _parent->getAbsolutePosition();

    // Recursively update every childs
    for (auto it = _childs.begin(), end = _childs.end(); it != end; ++it) {
        (*it)->update();
    }
}

vec3 Node::getPosition(void) const {
    return _position;
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

void Node::addChild(Node *node, bool fastInsert) {
    if (fastInsert || std::find(_childs.begin(), _childs.end(), node) == _childs.end()) {
        _childs.push_back(node);
        node->setParent(this);
    }
}

std::vector<Node*>& Node::getChilds(void) {
    return _childs;
}

void Node::setObjectId(uint32_t id) {
    _objectId = id;
}

uint32_t Node::getObjectId(void) const {
    return _objectId;
}