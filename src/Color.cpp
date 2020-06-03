#include "Color.h"
#include <algorithm>

Color::Color(float R, float G, float B)
{
    red   = std::clamp<float>(R, 0.f, 1.f);
    green = std::clamp<float>(G, 0.f, 1.f);
    blue  = std::clamp<float>(B, 0.f, 1.f);
}

Eigen::Vector3f Color::GetColor(Color::EColor color)
{
    Color col;
    switch (color)
    {
        case Color::EColor::BLUE:
            col = Color(0.f, 0.f, 1.f);
            break;
        case Color::EColor::GREEN:
            col = Color(0.f, 1.f, 0.f);
            break;
        case Color::EColor::RED:
            col = Color(1.f, 0.f, 0.f);
            break;
        case Color::EColor::YELLOW:
            col = Color(1.f, 1.f, 0.f);
        case Color::EColor::WHITE:
            col = Color(1.f, 1.f, 1.f);
            break;
    }
    return col.GetColor();
}
