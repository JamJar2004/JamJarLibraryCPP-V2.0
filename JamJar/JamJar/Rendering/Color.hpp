#pragma once

#include "../Numerics.hpp"

class Color
{
private:
	union
	{
		struct
		{
			UInt8 m_alpha;
			UInt8 m_red;
			UInt8 m_green;
			UInt8 m_blue;
		};

		UInt32 m_value;
	};
public:
	Color(UInt8 alpha, UInt8 red, UInt8 green, UInt8 blue) : m_alpha(alpha), m_red(red), m_green(green), m_blue(blue) {}

	Color(UInt32 value) : m_value(value) {}

	Color& InitFromARGB(Float32 alpha, Float32 red, Float32 green, Float32 blue)
	{
		m_alpha = UInt8(alpha * 255);
		m_red   = UInt8(red   * 255);
		m_green = UInt8(green * 255);
		m_blue  = UInt8(blue  * 255);

		return *this;
	}

	Color& InitFromTCMY(Float32 transparency, Float32 cyan, Float32 magenta, Float32 yellow)
	{
		m_alpha = UInt8((1.0f - transparency) * 255);
		m_red   = UInt8((1.0f - cyan        ) * 255);
		m_green = UInt8((1.0f - magenta     ) * 255);
		m_blue  = UInt8((1.0f - yellow      ) * 255);

		return *this;
	}

	Color& InitFromRGB(Float32 red , Float32 green  , Float32 blue  ) { return InitFromARGB(1.0f, red , green  , blue  ); }
	Color& InitFromCMY(Float32 cyan, Float32 magenta, Float32 yellow) { return InitFromTCMY(1.0f, cyan, magenta, yellow); }

	Color& InitFromHSL(Float32 hue, Float32 saturation, Float32 luminance)
	{

	}

	Float32 GetAlpha() const { return Float32(m_alpha) / 255.0f; }
	Float32 GetRed()   const { return Float32(m_red  ) / 255.0f; }
	Float32 GetGreen() const { return Float32(m_green) / 255.0f; }
	Float32 GetBlue()  const { return Float32(m_blue ) / 255.0f; }
};