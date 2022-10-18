#include "Color.hpp"

ColorF::ColorF(const Color& color) : 
	m_alpha(color.GetAlpha() / Float32(255.0f)),
	m_red  (color.GetRed()   / Float32(255.0f)),
	m_green(color.GetGreen() / Float32(255.0f)), 
	m_blue (color.GetBlue()  / Float32(255.0f))
{
}

ColorF ColorF::CreateFromARGB(Float32 alpha, Float32 red, Float32 green, Float32 blue) { return ColorF(alpha, red, green, blue); }

ColorF ColorF::CreateFromTCMY(Float32 transparency, Float32 cyan, Float32 magenta, Float32 yellow) 
{
    return ColorF(1.0f - transparency, 1.0f - cyan, 1.0f - magenta, 1.0f - yellow); 
}

ColorF ColorF::CreateFromRGB(Float32 red , Float32 green  , Float32 blue  ) { return CreateFromARGB(1.0f, red , green  , blue  ); }
ColorF ColorF::CreateFromCMY(Float32 cyan, Float32 magenta, Float32 yellow) { return CreateFromTCMY(0.0f, cyan, magenta, yellow); }

ColorF ColorF::CreateFromHSL(Float32 hue, Float32 saturation, Float32 luminance)
{
	Float32 r, g, b;

	Float32 h = hue % 360.0f;

	UInt32  section = UInt32(h / 60);
	Float32 varying = (h / 60.0f) - h;

	switch(section.ToRawValue())
	{
		case 0:
			r = 1.0f;
			g = varying;
			b = 0.0f;
			break;
		case 1:
			r = 1.0f - varying;
			g = 1.0f;
			b = 0.0f;
			break;
		case 2:
			r = 0.0f;
			g = 1.0f;
			b = varying;
			break;
		case 3:
			r = 0.0f;
			g = 1.0f - varying;
			b = 1.0f;
			break;
		case 4:
			r = varying;
			g = 0;
			b = 1.0f;
			break;
		case 5:
			r = 1.0f;
			g = 0;
			b = 1.0f - varying;
			break;
	}

	if(luminance <= 0.5f)
	{
		r *= (luminance * 2.0f);
		g *= (luminance * 2.0f);
		b *= (luminance * 2.0f);
	}
	else
	{
		Float32 value = (luminance - 0.5f) * 2.0f;
		r += value - (value * r);
		g += value - (value * g);
		b += value - (value * b);
	}

	r = (r * saturation) + (luminance * (1.0f - saturation));
	g = (g * saturation) + (luminance * (1.0f - saturation));
	b = (b * saturation) + (luminance * (1.0f - saturation));

	return CreateFromRGB(r, g, b);
}