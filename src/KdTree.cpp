//
//  KdTree.cpp
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#include "KdTree.h"

#include <iostream>
#include <algorithm>

#include "Renderer.h"

const uint8_t KdTree::Xaxis = 0;
const uint8_t KdTree::Yaxis = 1;
const uint8_t KdTree::Zaxis = 2;

KdTree::NodeComparator::NodeComparator(uint8_t axis) :
    _axis(axis)
{
}

KdTree::NodeComparator::~NodeComparator(void) {
}

bool KdTree::NodeComparator::operator()(Node *node1, Node *node2) {
    if (_axis == Xaxis)
        return node1->getBoundingBox().getCenter().x < node2->getBoundingBox().getCenter().x;
    else if (_axis == Yaxis)
        return node1->getBoundingBox().getCenter().y < node2->getBoundingBox().getCenter().y;
    else if (_axis == Zaxis)
        return node1->getBoundingBox().getCenter().z < node2->getBoundingBox().getCenter().z;
    return false;
}

KdTree::KdTree(void) :
    _maxDepth(20), _rootNode(NULL), _invalidNodes()
{
    
}

KdTree::~KdTree(void) {
    
}

void KdTree::setMaxDepth(uint32_t maxDepth) {
    _maxDepth = maxDepth;
}

void KdTree::build(const std::vector<Node*>& nodes) {
    
    if (nodes.empty())
        return ;
    
    // Get the nodes with bounding box only and create the box that wrap all objects
    std::vector<Node*>  validNodes;
    BoundingBox         rootBox;
    
    bool first = true;
    for (auto it = nodes.begin(), end = nodes.end(); it != end; ++it) {
        if ((*it)->hasBoundingBox()) {
            validNodes.push_back(*it);
            const BoundingBox& nodeBox = (*it)->getBoundingBox();
            
            if (first) {
                first = false;
                rootBox.min = nodeBox.min;
                rootBox.max = nodeBox.max;
            } else {
                rootBox.min.x = min(nodeBox.min.x, rootBox.min.x);
                rootBox.min.y = min(nodeBox.min.y, rootBox.min.y);
                rootBox.min.z = min(nodeBox.min.z, rootBox.min.z);
                
                rootBox.max.x = max(nodeBox.max.x, rootBox.max.x);
                rootBox.max.y = max(nodeBox.max.y, rootBox.max.y);
                rootBox.max.z = max(nodeBox.max.z, rootBox.max.z);
            }
        } else {
            _invalidNodes.push_back(*it);
        }
    }
    
//    d(rootBox.min);
//    d(rootBox.max);
    
    _rootNode = new TreeNode();
    _rootNode->box = rootBox;
    _rootNode->nodes = validNodes;
    _buildTreeNode(_rootNode);
}

void KdTree::_buildTreeNode(TreeNode* treeNode, uint32_t level) {
    if (level == _maxDepth || treeNode->nodes.size() <= 2) {
        return ;
    }
    
    uint32_t axis = level % 3;
    
    // Build left and right childs with the box separated in two equal boxes
    treeNode->leftChild = new TreeNode();
    treeNode->rightChild = new TreeNode();
    
    treeNode->leftChild->box = treeNode->box;
    treeNode->rightChild->box = treeNode->box;
    
    if (axis == Xaxis) {
        treeNode->leftChild->box.max.x = (treeNode->box.max.x + treeNode->box.min.x) / 2.0f;
        treeNode->rightChild->box.min.x = treeNode->leftChild->box.max.x;
    } else if (axis == Yaxis) {
        treeNode->leftChild->box.max.y = (treeNode->box.max.y + treeNode->box.min.y) / 2.0f;
        treeNode->rightChild->box.min.y = treeNode->leftChild->box.max.y;
    } else if (axis == Zaxis) {
        treeNode->leftChild->box.max.z = (treeNode->box.max.z + treeNode->box.min.z) / 2.0f;
        treeNode->rightChild->box.min.z = treeNode->leftChild->box.max.z;
    }
    
    // Fill childs with nodes within their boxes
    for (auto it = treeNode->nodes.begin(), end = treeNode->nodes.end(); it != end; ++it) {
        Node* node = *it;
        if (node->getBoundingBox().intersectWithBox(treeNode->leftChild->box))
            treeNode->leftChild->nodes.push_back(node);
        if (node->getBoundingBox().intersectWithBox(treeNode->rightChild->box))
            treeNode->rightChild->nodes.push_back(node);
    }
    _buildTreeNode(treeNode->leftChild, level + 1);
    _buildTreeNode(treeNode->rightChild, level + 1);
    treeNode->nodes.clear();
}

float KdTree::traceRay(const Ray& ray, Node*& intersectedNode) {
    // Trace ray through valid nodes
    float d = _traceRay(_rootNode, ray, intersectedNode);
    // And then invalid ones (unlimited objects for instance)
    _intersectWithNodes(_invalidNodes, ray, intersectedNode, d);
    return d;
}

bool KdTree::castShadow(const Ray& ray, float maxDist) {
    // Cast through valid nodes
    bool shadow = _castShadow(_rootNode, ray, maxDist);
    // And then invalid ones (unlimited objects for instance)
    if (!shadow)
        shadow = _castShadowWithNodes(_invalidNodes, ray, maxDist);
    return shadow;
}

void KdTree::_intersectWithNodes(std::vector<Node*>& nodes, const Ray& ray,
                                 Node*& intersectedNode, float& d) {
    Node* node = NULL;
    float minD = d;
    for (auto it = nodes.begin(), end = nodes.end(); it != end; ++it) {
        float tmpD = (*it)->intersectWithRay(ray);
        if (tmpD > Renderer::Epsilon && (minD < Renderer::Epsilon || tmpD < minD)) {
            minD = tmpD;
            node = *it;
        }
    }
    if (node) {
        intersectedNode = node;
        d = minD;
    }
}

float KdTree::_traceRay(TreeNode* treeNode, const Ray& ray, Node*& intersectedNode) {
    // Leaf node, we trace through the nodes
    if (!treeNode->leftChild || !treeNode->rightChild) {
        float d = -1.0f;
        _intersectWithNodes(treeNode->nodes, ray, intersectedNode, d);
        return d;
    }
    
    // Trace ray through the left and right child
    bool isInLeft = false;
    bool isInRight = false;
    float dLeft = treeNode->leftChild->box.intersectWithRay(ray, &isInLeft);
    float dRight = treeNode->rightChild->box.intersectWithRay(ray, &isInRight);
    
    if (dLeft < Renderer::Epsilon && dRight < Renderer::Epsilon)
        return -1.0f;
    if (dLeft > Renderer::Epsilon && dRight < Renderer::Epsilon)
        return _traceRay(treeNode->leftChild, ray, intersectedNode);
    if (dLeft < Renderer::Epsilon && dRight > Renderer::Epsilon)
        return _traceRay(treeNode->rightChild, ray, intersectedNode);
    if ((dRight - dLeft) > Renderer::Epsilon || isInLeft) {
        dLeft = _traceRay(treeNode->leftChild, ray, intersectedNode);
        if (dLeft > Renderer::Epsilon)
            return dLeft;
        return _traceRay(treeNode->rightChild, ray, intersectedNode);
    } else if ((dLeft - dRight) > Renderer::Epsilon || isInRight) {
        dRight = _traceRay(treeNode->rightChild, ray, intersectedNode);
        if (dRight > Renderer::Epsilon)
            return dRight;
        return _traceRay(treeNode->leftChild, ray, intersectedNode);
    } else {
        // We touch two boxes... very rare
        Node* interLeft = NULL;
        Node* interRight = NULL;
        dLeft = _traceRay(treeNode->leftChild, ray, interLeft);
        dRight = _traceRay(treeNode->rightChild, ray, interRight);
        if (dLeft > Renderer::Epsilon && (dRight < Renderer::Epsilon || dLeft < dRight)) {
            intersectedNode = interLeft;
            return dLeft;
        } else {
            intersectedNode = interRight;
            return dRight;
        }
    }
    return -1.0f;
}

bool KdTree::_castShadow(KdTree::TreeNode *treeNode, const Ray& ray, float maxDist) {
    // Leaf node, we trace through the nodes
    if (!treeNode->leftChild || !treeNode->rightChild) {
        return _castShadowWithNodes(treeNode->nodes, ray, maxDist);
    }
    
    // Trace ray through the left and right child
    bool isInLeft = false;
    bool isInRight = false;
    float dLeft = treeNode->leftChild->box.intersectWithRay(ray, &isInLeft);
    float dRight = treeNode->rightChild->box.intersectWithRay(ray, &isInRight);
    
    if (dLeft < Renderer::Epsilon && dRight < Renderer::Epsilon)
        return -1.0f;
    if (dLeft > Renderer::Epsilon && dRight < Renderer::Epsilon)
        return _castShadow(treeNode->leftChild, ray, maxDist);
    if (dLeft < Renderer::Epsilon && dRight > Renderer::Epsilon)
        return _castShadow(treeNode->rightChild, ray, maxDist);
    if ((dRight - dLeft) > Renderer::Epsilon || isInLeft) {
        dLeft = _castShadow(treeNode->leftChild, ray, maxDist);
        if (dLeft)
            return dLeft;
        return _castShadow(treeNode->rightChild, ray, maxDist);
    } else if ((dLeft - dRight) > Renderer::Epsilon || isInRight) {
        dRight = _castShadow(treeNode->rightChild, ray, maxDist);
        if (dRight)
            return dRight;
        return _castShadow(treeNode->leftChild, ray, maxDist);
    } else {
        // We touch two boxes... very rare
        if (_castShadow(treeNode->leftChild, ray, maxDist))
            return true;
        return _castShadow(treeNode->rightChild, ray, maxDist);
    }
    return false;
}

bool KdTree::_castShadowWithNodes(std::vector<Node *> &nodes, const Ray &ray, float maxDist) {
    for (auto it = nodes.begin(), end = nodes.end(); it != end; ++it) {
        if (!dynamic_cast<Object*>(*it))
            continue;
        float d = (*it)->intersectWithRay(ray);
        if (d > Renderer::Epsilon && d < maxDist) {
            return true;
        }
    }
    return false;
}