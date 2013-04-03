//
//  KdTree.h
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#ifndef __SimplePathTracer__KdTree__
#define __SimplePathTracer__KdTree__

#include <vector>

#include "Ray.h"
#include "Node.h"

class KdTree {
public:
    
    static const uint8_t Xaxis;
    static const uint8_t Yaxis;
    static const uint8_t Zaxis;
    
    struct TreeNode {
        BoundingBox         box;
        std::vector<Node*>  nodes;
        TreeNode*           leftChild;
        TreeNode*           rightChild;
    };
    
    class NodeComparator {
    public:
        NodeComparator(uint8_t axis);
        ~NodeComparator(void);
        
        bool operator()(Node* node1, Node* node2);
    private:
        uint8_t _axis;
    };
    
    KdTree(void);
    ~KdTree(void);
    
    void    setMaxDepth(uint32_t maxDepth);
    void    build(const std::vector<Node*>& nodes);
    float   traceRay(const Ray& ray, Node*& intersectedNode);
    bool    castShadow(const Ray& ray, float maxDist);

private:
    void    _buildTreeNode(TreeNode* treeNode, uint32_t level=0);
    float   _traceRay(TreeNode* treeNode, const Ray& ray,
                      Node*& intersectedNode);
    void    _intersectWithNodes(std::vector<Node*>& nodes, const Ray& ray,
                                Node*& intersectedNode, float& d);
    
    bool    _castShadow(TreeNode* treeNode, const Ray& ray, float maxDist);
    bool    _castShadowWithNodes(std::vector<Node*>& nodes, const Ray& ray,
                                 float maxDist);

    
    
    uint32_t            _maxDepth;
    TreeNode*           _rootNode;
    std::vector<Node*>  _invalidNodes;
};

#endif /* defined(__SimplePathTracer__KdTree__) */
