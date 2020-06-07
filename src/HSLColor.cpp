#include "HSLColor.h"
#include <algorithm>

static float makeRGB(float v1, float v2, float hue)
{
    if (hue < 0)
    {
        hue += 1;
    }
    if (hue > 1)
    {
        hue -= 1;
    }
    if (6.f * hue < 1)
    {
        return (v1 + (v2 - v1) * 6 * hue);
    }
    if (2.f * hue < 1)
    {
        return v2;
    }
    if (3.f * hue < 2)
    {
        return (v1 + (v2 - v1) * ((2.0 / 3.0) - hue) * 6);
    }
    return v1;
}

HSLColor::HSLColor(float hue, float saturation, float lightness)
{
    m_hue        = std::clamp(hue, 0.f, 360.f);
    m_saturation = std::clamp(saturation, 0.f, 100.f);
    m_lightness  = std::clamp(lightness, 0.f, 100.f);
}

Color HSLColor::ConvertHueToRGB()
{
    float h = m_hue / 360.f;
    float s = m_saturation / 100.f;
    float l = m_lightness / 100.f;

    float v1, v2;
    if (l < 0.5f)
    {
        v2 = l * (1 + s);
    }
    else
    {
        v2 = (l + s) - (s * l);
    }
    v1 = 2 * l - v2;

    float R = makeRGB(v1, v2, h + 1.f / 3.f);
    float G = makeRGB(v1, v2, h);
    float B = makeRGB(v1, v2, h - 1.f / 3.f);

    return Color(R, G, B);
}

Color HSLColor::ConvertHueToRGB(float hue, float saturation, float lightness)
{
    HSLColor col(hue, saturation, lightness);
    return col.ConvertHueToRGB();
}
