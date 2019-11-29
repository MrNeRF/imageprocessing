#include "Color.h"
#include <algorithm>

Color::Color(float R, float G, float B)
{
    red   = std::clamp<float>(R, 0.f, 1.f);
    green = std::clamp<float>(G, 0.f, 1.f);
    blue  = std::clamp<float>(B, 0.f, 1.f);
}
