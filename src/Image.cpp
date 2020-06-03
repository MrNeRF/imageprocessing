#include "Image.h"
#include <GL/glew.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
}

Image::~Image(void)
{
    stbi_image_free(data);
    data = nullptr;
}
