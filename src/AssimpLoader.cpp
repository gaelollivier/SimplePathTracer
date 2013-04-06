//
//  AssimpLoader.cpp
//  SimplePathTracer
//
//  Created by gael on 29/03/13.
//
//

#include "AssimpLoader.h"

#include "Face.h"

AssimpLoader::AssimpLoader(Material* defaultMaterial) :
    _importer(), _defaultMaterial(defaultMaterial)
{
}

AssimpLoader::~AssimpLoader(void) {
}

Node* AssimpLoader::loadFile(const std::string &filename) {
    // Import the scene from the file
    const aiScene* scene = _importer.ReadFile(filename,
                                              aiProcess_Triangulate
                                              | aiProcess_GenSmoothNormals
                                              | aiProcess_FixInfacingNormals);
    
    // If the import failed, report it
    if(!scene) {
        std::cerr << "AssimpLoader Error: " << _importer.GetErrorString() << std::endl;
        return NULL;
    }
    
    _nbFacesLoaded = 0;
    
    // Create the root node and import the scene !
    Node* rootNode = _loadAssimpNode(scene, scene->mRootNode);
    
    std::cout << _nbFacesLoaded << " faces loaded" << std::endl;
    
    return rootNode;
}

Node* AssimpLoader::_loadAssimpNode(const aiScene *scene, aiNode *assimpNode) {
    Node* node = new Node();
    
    node->setName(assimpNode->mName.data);
    
    // Load meshes of the node
    int nbAttr = 8;
    
    for (uint i = 0; i < assimpNode->mNumMeshes; ++i) {
        aiMesh* assimpMesh = scene->mMeshes[assimpNode->mMeshes[i]];
        
        if (!(assimpMesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) ||
            assimpMesh->mNumVertices <= 0)
            continue ;
        
        Node* mesh = new Node();
        
        mesh->setName(assimpMesh->mName.data);
        
        double *vertexes = new double[assimpMesh->mNumVertices * nbAttr];
        for (uint j = 0; j < assimpMesh->mNumVertices; ++j) {
            vertexes[j * nbAttr + 0] = assimpMesh->mVertices[j].x;
            vertexes[j * nbAttr + 1] = assimpMesh->mVertices[j].y;
            vertexes[j * nbAttr + 2] = assimpMesh->mVertices[j].z;
            if (assimpMesh->mNormals) {
                vertexes[j * nbAttr + 3] = assimpMesh->mNormals[j].x;
                vertexes[j * nbAttr + 4] = assimpMesh->mNormals[j].y;
                vertexes[j * nbAttr + 5] = assimpMesh->mNormals[j].z;
            }
            if (assimpMesh->mTextureCoords[0]) {
                vertexes[j * nbAttr + 6] = assimpMesh->mTextureCoords[0][j].x;
                vertexes[j * nbAttr + 7] = assimpMesh->mTextureCoords[0][j].y;
            }
        }
        _nbFacesLoaded += assimpMesh->mNumFaces;
        for (uint j = 0; j < assimpMesh->mNumFaces; ++j) {
            if (assimpMesh->mFaces[j].mNumIndices == 3) {
                vec3 v1(vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 0],
                        vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 1],
                        vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 2]);
                vec3 v2(vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 0],
                        vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 1],
                        vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 2]);
                vec3 v3(vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 0],
                        vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 1],
                        vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 2]);
                vec3 n1(vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 3],
                        vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 4],
                        vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 5]);
                vec3 n2(vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 3],
                        vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 4],
                        vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 5]);
                vec3 n3(vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 3],
                        vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 4],
                        vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 5]);
                vec2 t1(vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 6],
                        vertexes[assimpMesh->mFaces[j].mIndices[0] * nbAttr + 7]);
                vec2 t2(vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 6],
                        vertexes[assimpMesh->mFaces[j].mIndices[1] * nbAttr + 7]);
                vec2 t3(vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 6],
                        vertexes[assimpMesh->mFaces[j].mIndices[2] * nbAttr + 7]);
                v1 *= 1;
                v2 *= 1;
                v3 *= 1;
                Face* face = new Face(v1, v2, v3, _defaultMaterial);
                face->setObjectId(42);
                face->setNormals(n1, n2, n3);
                face->setTextureCoords(t1, t2, t3);
                mesh->addChild(face, true);
            }
        }
        delete[] vertexes;
        // Add the mesh to the node
        node->addChild(mesh);
    }
    // Load child nodes recursively
    for (uint i = 0; i < assimpNode->mNumChildren; ++i) {
        node->addChild(_loadAssimpNode(scene, assimpNode->mChildren[i]));
    }
    return node;
}