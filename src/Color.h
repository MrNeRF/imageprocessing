#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include <Eigen/Dense>

class Color
{
public:
    enum class EColor
    {
        BLUE,
        GREEN,
        RED,
        YELLOW
    };

public:
    Color() = default;
    Color(float R, float G, float B);
    Eigen::Vector3f        GetColor() const { return Eigen::Vector3f(red, green, blue); }
    static Eigen::Vector3f GetColor(Color::EColor color);

private:
    float red   = 0.5f;
    float green = 0.5f;
    float blue  = 0.5f;
};

#endif
