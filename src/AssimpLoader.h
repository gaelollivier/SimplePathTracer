//
//  AssimpLoader.h
//  SimplePathTracer
//
//  Created by gael on 29/03/13.
//
//

#ifndef __SimplePathTracer__AssimpLoader__
#define __SimplePathTracer__AssimpLoader__

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Node.h"
#include "Material.h"

class AssimpLoader {
public:
    
    AssimpLoader(Material* defaultMaterial=NULL);
    ~AssimpLoader(void);
    
    Node* loadFile(const std::string& filename);
    
private:
    
    Node* _loadAssimpNode(const aiScene* scene, aiNode* assimpNode);
    
    Assimp::Importer    _importer;
    Material*           _defaultMaterial;
};

#endif /* defined(__SimplePathTracer__AssimpLoader__) */
