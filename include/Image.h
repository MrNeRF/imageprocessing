#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image
{
public:
    Image(const std::string& path);
    ~Image(void) = default;
    int  GetImageID(void) { return m_textureID; };
    void Use();

private:
    void         init(const std::string& path);
    unsigned int m_textureID;
};

#endif
