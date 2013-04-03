//
//  Gui.h
//  SimplePathTracer
//
//  Created by gael on 03/04/13.
//
//

#ifndef __SimplePathTracer__Gui__
#define __SimplePathTracer__Gui__

#include <SFML/Graphics.hpp>

#include "Renderer.h"

class Gui {
public:
    
    static void show(const Renderer& renderer);
    static void saveImage(const Renderer& renderer, const std::string& filename);
    
private:
    Gui(void);
    ~Gui(void);
    
    void _init(const vec2& size);
    void _resize(const vec2& size);
    void _resizeImage(const vec2& size);
    
    // Unique instance
    static Gui  _instance;
    
    // Private members
    bool                _isInit;
    sf::RenderWindow*   _window;
    vec2                _windowSize;
    sf::Image           _image;
    sf::Texture         _texture;
    sf::Sprite          _sprite;
};

#endif /* defined(__SimplePathTracer__Gui__) */
