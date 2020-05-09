#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <string>

class Image
{
public:
    Image(const std::string& path);
    ~Image(void);

    unsigned char* data = nullptr;

    int width;
    int height;
    int nrChannels;
};

#endif
