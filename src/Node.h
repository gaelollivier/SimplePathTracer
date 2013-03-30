//
//  Node.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Node__
#define __SimplePathTracer__Node__

#include "Ray.h"
#include "BoundingBox.h"

#include <vector>

class Node {
public:
    
    Node(vec3 position=vec3());
    
    virtual ~Node(void);
    
    virtual void update(void);
    
    vec3 getPosition(void) const;
    vec3 getAbsolutePosition(void) const;
    
    virtual float intersectWithRay(const Ray& ray);
    
    virtual bool hasBoundingBox(void) const;
    virtual const BoundingBox& getBoundingBox(void) const;
    
    void setParent(Node* node);
    Node* getParent(void);
    
    void addChild(Node* node, bool fastInsert=false);
    std::vector<Node*>& getChilds(void);
    
    void setObjectId(uint32_t id);
    uint32_t getObjectId(void) const;
    
protected:
    Node*               _parent;
    std::vector<Node*>  _childs;
    vec3                _position;
    vec3                _absolutePosition;
    BoundingBox         _boundingBox;
    uint32_t            _objectId;
};

#endif /* defined(__SimplePathTracer__Node__) */
