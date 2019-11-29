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
    float red   = 0.0f;
    float green = 0.0f;
    float blue  = 0.0f;
};

#endif
