#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include <Eigen/Dense>

class Color
{
public:
    Color(void) = default;
    Color(float R, float G, float B);
    Eigen::Vector3f GetColor() const { return Eigen::Vector3f(red, green, blue); }

private:
    float red   = 155.0f;
    float green = 155.0f;
    float blue  = 155.0f;
};

#endif
