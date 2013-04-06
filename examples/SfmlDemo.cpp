//
//  Sfml.cpp
//  SimplePathTracer
//
//  Created by gael on 01/04/13.
//
//

#include <SFML/Graphics.hpp>

#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Face.h"
#include "Light.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"
#include "AssimpLoader.h"
#include "RenderingSession.h"
#include "Gui.h"

void cornellBox(Scene* scene, vec2 cameraSize);
void manySpheres(Scene* scene, vec2 cameraSize);
void basicScene(Scene* scene, vec2 cameraSize);
void assimpLoader(Scene* scene, vec2 cameraSize);
void standfordDragon(Scene* scene, vec2 cameraSize);

int main(int, char**) {
    
    sf::Clock mainClock;
    
    vec2 cameraSize = vec2(1280, 720);
    vec2 imageSize = cameraSize / 2.0f;
    
    Scene* scene = new Scene();
    
    std::cout << "Generating scene...\n";

    standfordDragon(scene, cameraSize);
    //assimpLoader(scene, cameraSize);
    
    Node::List objects = scene->getRootNode()->search("S_Studio_Plane");
    for (auto it = objects.begin(), end = objects.end(); it != end; ++it) {
        (*it)->remove();
        d(Debug::Detail::ToStringPtr(*it));
    }
    
    scene->update();
    
    //d(Debug::ToString(*scene));
    
    RenderingSession session;
    
    session.setScene(scene);
    session.setRenderer(new Renderer());
    session.setResolution(imageSize);
    session.setNbSamples(100);
    session.setNbThreads(20);
    session.setUseBVH(true);
    session.setBVHMaxDepth(25);
    
    std::cout << "Building BVH...\n";
    
    session.getRenderer()->buildBVH();
    
    std::cout << "Done\n";
    
    while (session.getRenderedSamples() < session.getNbSamples()) {
        
        // Render the scene
        session.render();
        
        Gui::show(session);
        Gui::saveImage(session, "output.png");
        
        session.setTotalEllapsedTime(mainClock.getElapsedTime().asSeconds());
        
        std::cout << session.getStats() << std::endl;
    }
        
    return 0;
}



static vec3 hexColor(uint32_t color) {
    uint8_t* composants = (uint8_t*)&color;
    return vec3((float)composants[2] / 255.0f,
                (float)composants[1] / 255.0f,
                (float)composants[0] / 255.0f);
}

void cornellBox(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    
    Material* white = new Material(vec3(1, 1, 1));
    Material* reflect = new Material(vec3(1, 1, 1));
    Material* refract = new Material(vec3(1, 1, 1));
    Material* red = new Material(vec3(hexColor(0xDE1826)));
    Material* blue = new Material(vec3(hexColor(0x1899DE)));
    Material* yellow = new Material(vec3(hexColor(0xEDDD28)));
    
    reflect->setReflection(1);
    reflect->setGlossiness(0.3);
    refract->setRefraction(1);
    
    red->setSpecular(0);
    
    // Floor
    *scene << new Plane(vec3(0, 0, 0), vec3(0, 1, 0), white);
    // Back
    *scene << new Plane(vec3(0, 0, 0), vec3(0, 0, 1), white);
    // Left
    *scene << new Plane(vec3(-10, 0, 0), vec3(1, 0, 0), red);
    // Right
    *scene << new Plane(vec3(10, 0, 0), vec3(-1, 0, 0), blue);
    // Roof
    *scene << new Plane(vec3(0, 20, 0), vec3(0, -1, 0), white);
    
    // Objects
    *scene << new Sphere(vec3(5, 3, 5), 3, refract);
    *scene << new Sphere(vec3(-6, 3, 15), 3, yellow);
    *scene << new Cube(vec3(-8, 0, 5), vec3(0, 10, 10), reflect);
    
    *scene << new Light(vec3(0, 20, 3), vec3(1, 1, 1), 5);
    
    *scene << new Camera(vec2(cameraSize), vec3(5, 10, 40), vec3(0, 8, 0));
}

void basicScene(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    
    uint32_t colors[] = {
        0xFFFFFF,
        0xDE1826,
        0x1899DE,
        0xEDDD28
    };
    
    std::vector<Material*> materials;
    
    for (uint32_t i = 0; i < sizeof(colors) / sizeof(uint32_t); ++i) {
        materials.push_back(new Material(hexColor(colors[i])));
        // Color with reflect
        Material* reflectMaterial = new Material(hexColor(colors[i]));
        reflectMaterial->setReflection(1);
        reflectMaterial->setGlossiness(0.3);
        materials.push_back(reflectMaterial);
    }
    
    // Floor
    *scene << new Plane(vec3(0, 0, 0), vec3(0, 1, 0), materials[0]);
    
    srandom(42 + 1);
    
    // Cubes
    Material* mat;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            Node* node = new Node("object");
            mat = materials[(i * 5 + j) % materials.size()];
            Cube* cube = new Cube(vec3(-10 + i * 5, 1.5, -15 + j * 6), 3, mat);
            cube->setName("cube");
            node->addChild(cube);
            mat = materials[(i * 5 + j + 2) % materials.size()];
            Sphere* sphere = new Sphere(vec3(-10 + i * 5, 4.5, -15 + j * 6), 1.5, mat);
            sphere->setName("sphere");
            node->addChild(sphere);
            node->setPosition(vec3(0, -2, 0));
            *scene << node;
        }
    }
    
    *scene << new Light(vec3(0, 20, 3), vec3(1, 1, 1), 5);
    
    *scene << new Camera(vec2(cameraSize), vec3(0, 15, 25), vec3(0, 0, 0));
}

void manySpheres(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    
    uint32_t colors[] = {
        0xFFFFFF,
        0xDE1826,
        0x1899DE,
        0xEDDD28
    };
    
    std::vector<Material*> materials;
    
    for (uint32_t i = 0; i < sizeof(colors) / sizeof(uint32_t); ++i) {
        materials.push_back(new Material(hexColor(colors[i])));
        // Color with reflect
        //materials.push_back(new Material(hexColor(colors[i]), 1, 0, 0, 0, 0.3));
    }
    
    // Floor
    *scene << new Plane(vec3(0, -10, 0), vec3(0, 1, 0), materials[0]);
    
    srandom(42 + 1);
    
    // Cubes
    Material* mat;
    int gridSize = 317;
    float sphereRadius = 2;
    float space = 2.0f*sphereRadius + 2.0f;
    
    float totalGridSize = (gridSize-1)*space;
    for (int i = 0; i < gridSize; ++i) {
        std::cout << i << "\n";
        for (int j = 0; j < gridSize; ++j) {
            mat = materials[(i * 5 + j + 2) % materials.size()];
            *scene << new Sphere(vec3((-totalGridSize/2.0f) + i * space,
                                      -10 + ((float)random() / RAND_MAX) * 500.0,
                                      -j*space), sphereRadius, mat);
        }
    }
    
    *scene << new Light(vec3(0, 20, 3), vec3(1, 1, 1), 5);
    
    *scene << new Camera(vec2(cameraSize), vec3(0, 10, 300), vec3(0, 100, 0));
}

void standfordDragon(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    scene->setBackgroundColor(hexColor(0x000000));
    
    Material* objectMaterial = new Material(hexColor(0xffffff));
    
    objectMaterial->setRefraction(1);
    objectMaterial->setDiffuse(1);
    objectMaterial->setSpecular(10);
    objectMaterial->setShininess(200);
    
    Material* floorMaterial = new Material(hexColor(0xffffff));
    floorMaterial->setReflection(0);
    floorMaterial->setGlossiness(0.6);
    floorMaterial->setDiffuse(1);
    floorMaterial->setSpecular(1);
    
    // Floor
    *scene << new Plane(vec3(0, 0, 0), vec3(0, 1, 0), floorMaterial);
    
    AssimpLoader loader(objectMaterial);
    
    std::string modelFile = "/Users/Gael/Dev/SimplePathTracer/models/stanford_dragon/dragon.obj";
    Node* model = loader.loadFile(modelFile);
    
    if (model) {
        //model->setPosition(vec3(0, 0, 0));
        //model->setScale(vec3(1, 1, 1));
        //model->setRotation(45, vec3(0, 1, 0));
        *scene << model;
    }
    else {
        std::cerr << "Error: Cannot load model " << modelFile << std::endl;
        exit(EXIT_FAILURE);
    }
    
    *scene << new Light(vec3(0, 22, -10), hexColor(0xffffff), 5);
    
    Camera* camera = new Camera(vec2(cameraSize), vec3(0, 5, 20), vec3(0, 3.5, 0));
    camera->setAperture(0);
    camera->setFocusDistance(23);
    *scene << camera;
}

void assimpLoader(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    scene->setBackgroundColor(hexColor(0x000000));
    
    Material* objectMaterial = new Material(hexColor(0xffffff));
    
    //objectMaterial->setReflection(1);
    //objectMaterial->setRefraction(1);
    objectMaterial->setDiffuse(1);
    objectMaterial->setSpecular(10);
    objectMaterial->setShininess(100);
    
    Material* floorMaterial = new Material(hexColor(0xffffff));
    floorMaterial->setReflection(1);
    floorMaterial->setGlossiness(0.1);
    floorMaterial->setDiffuse(1);
    floorMaterial->setSpecular(0);
    
    // Floor
    *scene << new Plane(vec3(0, 0, 0), vec3(0, 1, 0), floorMaterial);
    
    AssimpLoader loader(objectMaterial);
    
    std::string modelFile = "/Users/Gael/Desktop/models/Canon_EOS/Canon_EOS.obj";
    Node* model = loader.loadFile(modelFile);
    
    if (model)
        *scene << model;
    else {
        std::cerr << "Error: Cannot load model " << modelFile << std::endl;
        exit(EXIT_FAILURE);
    }
    
    *scene << new Light(vec3(0, 30, 20), hexColor(0xffffff), 5);
    
    Camera* camera = new Camera(vec2(cameraSize), vec3(-14, 10, 18), vec3(-2, 5, 0));
    camera->setAperture(0);
    camera->setFocusDistance(23);
    *scene << camera;
}