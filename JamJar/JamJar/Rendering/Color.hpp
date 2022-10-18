#pragma once

#include "../String.hpp"

class Color;

class ColorF
{
private:
	Float32 m_alpha;
	Float32 m_red;
	Float32 m_green;
	Float32 m_blue;

	ColorF(Float32 alpha, Float32 red, Float32 green, Float32 blue) : m_alpha(alpha), m_red(red), m_green(green), m_blue(blue) {}
public:
	static ColorF CreateFromARGB(Float32 alpha, Float32 red, Float32 green, Float32 blue);
	static ColorF CreateFromTCMY(Float32 transparency, Float32 cyan, Float32 magenta, Float32 yellow);

	static ColorF CreateFromRGB(Float32 red, Float32 green, Float32 blue);
	static ColorF CreateFromCMY(Float32 cyan, Float32 magenta, Float32 yellow);

	static ColorF CreateFromHSL(Float32 hue, Float32 saturation, Float32 luminance);

	ColorF(const Color& color);

	Float32 GetAlpha() const { return m_alpha; }
	Float32 GetRed()   const { return m_red;   }
	Float32 GetGreen() const { return m_green; }
	Float32 GetBlue()  const { return m_blue;  }

	Float32 GetTransparency() const { return 1.0f - m_alpha; }
	Float32 GetCyan()         const { return 1.0f - m_red;   }
	Float32 GetMagenta()      const { return 1.0f - m_green; }
	Float32 GetYellow()       const { return 1.0f - m_blue;  }

	Float32 GetHue() const 
	{
		Float32 hue;

		Float32 min = m_red.Min(m_green.Min(m_blue));
		Float32 max = m_red.Max(m_green.Max(m_blue));

		Float32 diff = max - min;

		if(max == m_red)
			hue = (m_green - m_blue) / diff + (m_green < m_blue ? 6 : 0);
		else if(max == m_green)
			hue = (m_blue - m_red) / diff + 2;
		else if(max == m_blue)
			hue = (m_red - m_green) / diff + 4;

		return hue;
	}

	Float32 GetSaturation() const
	{
		Float32 sat;

		Float32 min = m_red.Min(m_green.Min(m_blue));
		Float32 max = m_red.Max(m_green.Max(m_blue));

		Float32 diff = max - min;

		sat = diff / max;

		return sat;
	}

	Float32 GetLuminance() const
	{
		Float32 lum;

		Float32 min = m_red.Min(m_green.Min(m_blue));
		Float32 max = m_red.Max(m_green.Max(m_blue));

		lum = (max + min) / 2.0f;

		return lum;
	}

	void SetAlpha(Float32 alpha) { m_alpha = alpha; }
	void SetRed  (Float32 red  ) { m_red   = red;   }
	void SetGreen(Float32 green) { m_green = green; }
	void SetBlue (Float32 blue ) { m_blue  = blue;  }

	void SetTransparency(Float32 transparency) { m_alpha = 1.0f - transparency; }
	void SetCyan        (Float32 cyan        ) { m_red   = 1.0f - cyan;         }
	void SetMagenta     (Float32 magenta     ) { m_green = 1.0f - magenta;      }
	void SetYellow      (Float32 yellow      ) { m_blue  = 1.0f - yellow;       }

	ColorF operator+(const ColorF& other) { return ColorF(m_alpha + other.m_alpha, m_red + other.m_red, m_green + other.m_green, m_blue + other.m_blue); }
	ColorF operator*(const ColorF& other) { return ColorF(m_alpha * other.m_alpha, m_red * other.m_red, m_green * other.m_green, m_blue * other.m_blue); }

	ColorF operator+(Float32 other) { return ColorF(m_alpha + other, m_red + other, m_green + other, m_blue + other); }
	ColorF operator*(Float32 other) { return ColorF(m_alpha * other, m_red * other, m_green * other, m_blue * other); }

	String ToString() const { return "Color(" + m_alpha + ", " + m_red + ", " + m_green + ", " + m_blue + ")"; }
};

class Color
{
	union
	{
		struct
		{
			UInt8 m_alpha;
			UInt8 m_red;
			UInt8 m_green;
			UInt8 m_blue;
		};

		UInt32 m_number;
	};
public:
	Color(const ColorF& color) : 
		m_alpha(UInt8(color.GetAlpha().Clamp(0.0f, 1.0f) * 255)), 
		m_red  (UInt8(  color.GetRed().Clamp(0.0f, 1.0f) * 255)),
		m_green(UInt8(color.GetGreen().Clamp(0.0f, 1.0f) * 255)), 
		m_blue (UInt8( color.GetBlue().Clamp(0.0f, 1.0f) * 255)) {}

	Color(UInt32 number) : m_number(number) {}

	UInt8 GetAlpha() const { return m_alpha; }
	UInt8 GetRed()   const { return m_red;   }
	UInt8 GetGreen() const { return m_green; }
	UInt8 GetBlue()  const { return m_blue;  }

	UInt32 ToNumber() const { return m_number; }

	String ToString() const { return ColorF(*this).ToString(); }
};