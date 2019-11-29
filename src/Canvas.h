#ifndef CANVAS_H
#define CANVAS_H

#include "Image.h"
#include "Shader.h"
#include "TextureObject.h"
#include <memory>

class TextureObject;
class Rectangle;

class Canvas
{
public:
    Canvas(const Image& image);

    void Draw(void);

private:
    std::unique_ptr<Shader>        spImageShader;
    std::unique_ptr<TextureObject> spTextureObject;
    std::unique_ptr<Rectangle>     spRectangle;
};

#endif
