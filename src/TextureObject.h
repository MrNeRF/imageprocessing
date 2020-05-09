#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include "Image.h"

class TextureObject
{
public:
    TextureObject(const Image& image);
    void Bind(void);

private:
    unsigned int textureID;
};

#endif
