#include "Color.hpp"

RGBColor::operator HSLColor()
{
    Float32 hue, sat, lum;

    Float32 min = m_red.Min(m_green.Min(m_blue));
    Float32 max = m_red.Max(m_green.Max(m_blue));
    
    Float32 diff = max - min;

    if(max == m_red)
        hue = (m_green - m_blue) / diff + (m_green < m_blue ? 6 : 0);
    else if(max == m_green)
        hue = (m_blue - m_red) / diff + 2;
    else if(max == m_blue)
        hue = (m_red - m_green) / diff + 4;

    sat = diff / max;
    lum = (max + min) / 2.0f;

    return HSLColor(hue, sat, lum);
}
