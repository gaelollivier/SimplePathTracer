//
//  Gui.cpp
//  SimplePathTracer
//
//  Created by gael on 03/04/13.
//
//

#include "Gui.h"

Gui Gui::_instance;

void Gui::show(const RenderingSession& session) {
    const Image& image = session.getRenderBuffer();
    vec2 size = image.getSize();
    
    if (!Gui::_instance._isInit) {
        Gui::_instance._init(size);
    }
    else if (Gui::_instance._windowSize != image.getSize()) {
        Gui::_instance._resize(size);
    }
    
    // Copy the image data to the sf::Image
    for (float x = 0; x < size.x; ++x) {
        for (float y = 0; y < size.y; ++y) {
            vec3 color = image.getPixel(vec2(x, y)) / (float)session.getRenderedSamples();
            Gui::_instance._image.setPixel(x, y, sf::Color(color.r * 255,
                                                           color.g * 255,
                                                           color.b * 255));
        }
    }
    // Draw the image into the window
    Gui::_instance._texture.update(Gui::_instance._image);
    Gui::_instance._window->draw(Gui::_instance._sprite);
    
    // Display the window
    Gui::_instance._window->display();
}

void Gui::saveImage(const RenderingSession& session, const std::string& filename) {
    const Image& image = session.getRenderBuffer();
    vec2 size = image.getSize();
    
    Gui::_instance._resizeImage(size);
    // Copy the image data to the sf::Image
    for (float x = 0; x < size.x; ++x) {
        for (float y = 0; y < size.y; ++y) {
            vec3 color = image.getPixel(vec2(x, y)) / (float)session.getRenderedSamples();
            Gui::_instance._image.setPixel(x, y, sf::Color(color.r * 255,
                                                           color.g * 255,
                                                           color.b * 255));
        }
    }
    Gui::_instance._image.saveToFile(filename);    
}

Gui::Gui(void) :
    _isInit(false), _window(NULL), _windowSize(),
    _image(), _texture(), _sprite()
{
}

Gui::~Gui(void) {
    if (_window)
        delete _window;
}

void Gui::_init(const vec2& size) {
    _windowSize = size;
    
    _window = new sf::RenderWindow(sf::VideoMode(_windowSize.x, _windowSize.y),
                                   "SimplePathtracer");

    _image.create(_windowSize.x, _windowSize.y);
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);
    
    _isInit = true;
}

void Gui::_resize(const vec2& size) {
    if (size == _windowSize)
        return ;
    if (_window)
        _window->setSize(sf::Vector2u(size.x, size.y));
    
    _resizeImage(size);
    _texture.loadFromImage(_image);
    _sprite.setTexture(_texture);
    
    _windowSize = size;
}

void Gui::_resizeImage(const vec2 &size) {
    _image.create(size.x, size.y);
}