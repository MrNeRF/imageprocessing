#ifndef HSL_COLOR
#define HSL_COLOR

#include "Color.h"

class HSLColor
{
public:
    HSLColor(float hue, float saturation, float lightness);
    Color        ConvertHueToRGB();
    static Color ConvertHueToRGB(float hue, float saturation, float lightness);

private:
    float m_hue        = 0.f;
    float m_saturation = 0.f;
    float m_lightness  = 0.f;
};

#endif
