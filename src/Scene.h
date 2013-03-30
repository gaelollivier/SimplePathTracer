//
//  Scene.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Scene__
#define __SimplePathTracer__Scene__

#include <vector>

#include "Node.h"
#include "Object.h"
#include "Light.h"
#include "Camera.h"

class Scene {
public:
    
    Scene(void);
    ~Scene(void);
    
    // Prepare scene for rendering
    void    update(void);
    
    void    setExposure(float exposure);
    float   getExposure(void) const;
    
    void    setBackgroundColor(const vec3& color);
    vec3    getBackgroundColor(void) const;
    
    void addNode(Node* node);
    void removeNode(Node* node);
    
    Scene& operator<<(Node* node);
    
    Camera* getCurrentCamera(void);
    Node*   getRootNode(void);
    
    std::vector<Node*>&     getNodes(void);
    std::vector<Light*>&    getLights(void);

private:
    
    void    _addNodeToLists(Node* node);
    
    Node*                   _rootNode;
    std::vector<Node*>      _nodes;
    std::vector<Light*>     _lights;
    
    Camera*                 _currentCamera;
    float                   _exposure;
    vec3                    _backgroundColor;
};

#endif /* defined(__SimplePathTracer__Scene__) */
