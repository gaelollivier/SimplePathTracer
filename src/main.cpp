//
//  main.cpp
//  SimpleRenderer
//
//  Created by gael on 26/03/13.
//
//

#include <iostream>
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

static vec3 hexColor(uint32_t color) {
    uint8_t* composants = (uint8_t*)&color;
    return vec3((float)composants[2] / 255.0f,
                (float)composants[1] / 255.0f,
                (float)composants[0] / 255.0f);
}

void cornellBox(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    
    Material* white = new Material(vec3(1, 1, 1));
    Material* reflect = new Material(vec3(1, 1, 1), 1, 0, 0, 0, 0.3);
    Material* refract = new Material(vec3(1, 1, 1), 0, 1, 0, 0);
    Material* red = new Material(vec3(hexColor(0xDE1826)));
    Material* blue = new Material(vec3(hexColor(0x1899DE)));
    Material* yellow = new Material(vec3(hexColor(0xEDDD28)));
    
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
    
    *scene << new Camera(vec3(5, 10, 40), vec3(0, 8, 0), vec2(cameraSize));
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
        materials.push_back(new Material(hexColor(colors[i]), 1, 0, 0, 0, 0.3));
    }

    // Floor
    *scene << new Plane(vec3(0, 0, 0), vec3(0, 1, 0), materials[0]);
    
    srandom(42 + 1);
    
    // Cubes
    Material* mat;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            mat = materials[(i * 5 + j) % materials.size()];
            *scene << new Cube(vec3(-10 + i * 5, 1.5, -15 + j * 6), 3, mat);
            mat = materials[(i * 5 + j + 2) % materials.size()];
            *scene << new Sphere(vec3(-10 + i * 5, 4.5, -15 + j * 6), 1.5, mat);
        }
    }

    *scene << new Light(vec3(0, 20, 3), vec3(1, 1, 1), 5);
    
    *scene << new Camera(vec3(0, 15, 25), vec3(0, 0, 0), vec2(cameraSize));
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
    
    *scene << new Camera(vec3(0, 10, 300), vec3(0, 100, 0), vec2(cameraSize));
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
    
    std::string modelFile = "/Users/Gael/Desktop/models/stanford_dragon/dragon.obj";
    Node* model = loader.loadFile(modelFile);

    if (model)
        *scene << model;
    else {
        std::cerr << "Error: Cannot load model " << modelFile << std::endl;
        exit(EXIT_FAILURE);
    }
    
    *scene << new Light(vec3(0, 22, -10), hexColor(0xffffff), 5);
    
    Camera* camera = new Camera(vec3(0, 5, 20), vec3(0, 3.5, 0), vec2(cameraSize));
    camera->setAperture(0);
    camera->setFocusDistance(23);
    *scene << camera;
}

void assimpLoader(Scene* scene, vec2 cameraSize) {
    scene->setExposure(2);
    scene->setBackgroundColor(hexColor(0x000000));
    
    Material* objectMaterial = new Material(hexColor(0xffffff));
    
    //objectMaterial->setReflection(1);
    objectMaterial->setRefraction(1);
    objectMaterial->setDiffuse(0);
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
    
    *scene << new Light(vec3(0, 30, -20), hexColor(0xffffff), 5);
    
    Camera* camera = new Camera(vec3(-14, 10, 18), vec3(-2, 5, 0), vec2(cameraSize));
    camera->setAperture(0);
    camera->setFocusDistance(23);
    *scene << camera;
}

int main(int, char**) {
    
    vec2 cameraSize = vec2(1280, 720);
    vec2 imageSize = vec2(2880, 1800);
    
    Scene* scene = new Scene();
    
    std::cout << "Generating scene...\n";
    //manySpheres(scene, cameraSize);
    //cornellBox(scene, cameraSize);
    
    //assimpLoader(scene, cameraSize);
    standfordDragon(scene, cameraSize);
    
    Renderer renderer(imageSize);
    
    scene->update();
    renderer.setScene(scene);
    
    std::cout << "Generating tree...\n";
    renderer.buildKdTree();
    
    std::cout << "Done\n";
    
    
    sf::RenderWindow window(sf::VideoMode(imageSize.x,
                                          imageSize.y),
                            "SimplePathtracer");
    
    
    sf::Image resultImage;
    resultImage.create(imageSize.x, imageSize.y);
    
    sf::Texture resultTexture;
    resultTexture.loadFromImage(resultImage);
    
    sf::Sprite resultSprite;
    resultSprite.setTexture(resultTexture);
    
    sf::Clock mainClock;
    
    uint32_t nbPasses = 0;
    uint32_t avgTimePerFrame = 0;
  
    while (window.isOpen()) {        
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        sf::Clock frameClock;
        
        nbPasses += 1;
        
        // Render the scene
        renderer.render();
        
        // Copy result to sfml image
        vec3* result = renderer.getResultBuffer();
        for (float x = 0; x < imageSize.x; ++x) {
            for (float y = 0; y < imageSize.y; ++y) {
                vec3 color = result[(int)(y * imageSize.x + x)] / (float)nbPasses;
                resultImage.setPixel(x, y, sf::Color(color.r * 255, color.g * 255, color.b * 255));
            }
        }
        
        resultTexture.update(resultImage);
        
        resultImage.saveToFile("/Users/Gael/Desktop/output.png");
        window.draw(resultSprite);
        window.display();
        
        avgTimePerFrame += frameClock.getElapsedTime().asMilliseconds();
        
        std::cout << "Time to render frame: " << frameClock.getElapsedTime().asMilliseconds() << "ms"
        << ", Total ellapsed time:" << (int)mainClock.getElapsedTime().asSeconds() << "s"
        << ", " << nbPasses << " passes" << ", Average time per frame: " << (avgTimePerFrame / nbPasses) << "ms" << std::endl;
    }
    
    resultImage.saveToFile("/Users/Gael/Desktop/output.png");
    
    return 0;
}
