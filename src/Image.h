//
//  Image.h
//  SimplePathTracer
//
//  Created by gael on 03/04/13.
//
//

#ifndef __SimplePathTracer__Image__
#define __SimplePathTracer__Image__

#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class Image {
public:
    Image(void);
    ~Image(void);
    
    void clear(void);
    
    void setSize(const vec2& size);
    vec2 getSize(void) const;
    
    const std::vector<vec3>&    getData(void) const;
    
    vec3 getPixel(vec2 position) const;
    void setPixel(vec2 position, vec3 value);
    
private:
    vec2                    _size;
    std::vector<vec3>       _data;
};

#endif /* defined(__SimplePathTracer__Image__) */
