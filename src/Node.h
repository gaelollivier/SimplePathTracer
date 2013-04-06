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
    
    typedef std::vector<Node*> List;
    
    Node(std::string name="");
    Node(vec3 position);
    
    virtual ~Node(void);
    
    virtual void update(void);
    
    std::string getName(void) const;
    void        setName(const std::string name);
    
    void setPosition(const vec3 position);
    vec3 getPosition(void) const;
    
    void setScale(const vec3 scale);
    vec3 getScale(void) const;
    
    void setRotation(float angle, const vec3& axis);
    
    const mat4& getTransformationMatrix(void);
    vec3 getAbsolutePosition(void) const;
    
    virtual float intersectWithRay(const Ray& ray);
    
    virtual bool hasBoundingBox(void) const;
    virtual const BoundingBox& getBoundingBox(void) const;
    
    void setParent(Node* node);
    Node* getParent(void);
    
    void addChild(Node* node);
    void addChild(Node* node, bool fastInsert);
    void removeChild(Node* node);
    std::vector<Node*>& getChilds(void);
    const std::vector<Node*>& getChilds(void) const;
    
    //! Remove the node from the scene graph
    void remove(void);
    
    //! Recursively search for a node matching the given regex
    Node::List search(const std::string& regex);
    
    void setObjectId(uint32_t id);
    uint32_t getObjectId(void) const;
    
protected:
    std::string         _name;
    Node*               _parent;
    std::vector<Node*>  _childs;
    vec3                _position;
    vec3                _scale;
    mat4                _rotationMatrix;
    mat4                _transformationMatrix;
    vec3                _absolutePosition;
    BoundingBox         _boundingBox;
    uint32_t            _objectId;
};

#endif /* defined(__SimplePathTracer__Node__) */
