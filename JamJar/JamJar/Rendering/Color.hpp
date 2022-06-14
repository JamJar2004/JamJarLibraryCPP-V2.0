#pragma once

#include "../Numerics.hpp"

class HSLColor;

class RGBColor
{
private:
	Float32 m_red;
	Float32 m_green;
	Float32 m_blue;
public:
	RGBColor(Float32 red, Float32 green, Float32 blue) : m_red(red), m_green(green), m_blue(blue) {}

	explicit RGBColor(Float32 shade) : m_red(shade), m_green(shade), m_blue(shade) {}

	explicit operator HSLColor();

	Float32 GetRed()   const { return m_red;   }
	Float32 GetGreen() const { return m_green; }
	Float32 GetBlue()  const { return m_blue;  }

	void SetRed  (Float32 red  ) { m_red   = red;   }
	void SetGreen(Float32 green) { m_green = green; }
	void SetBlue (Float32 blue ) { m_blue  = blue;  }
};

class ARGBColor : public RGBColor
{
private:
	Float32 m_alpha;
public:
	ARGBColor(Float32 alpha, Float32 red, Float32 green, Float32 blue) : RGBColor(red, green, blue), m_alpha(alpha) {}
	
	ARGBColor(Float32 alpha, const RGBColor& baseColor) : RGBColor(baseColor), m_alpha(alpha) {}

	explicit ARGBColor(const RGBColor& baseColor) : RGBColor(baseColor), m_alpha(1.0f) {}

	Float32 GetAlpha() const { return m_alpha; }

	void SetAlpha(Float32 alpha) { m_alpha = alpha; }
};

class HSLColor
{
private:
	Float32 m_hue;
	Float32 m_sat;
	Float32 m_lum;
public:
	HSLColor(Float32 hue, Float32 saturation, Float32 luminance) : m_hue(hue), m_sat(saturation), m_lum(luminance) {}

	explicit HSLColor(const RGBColor& rgbColor) {}

	explicit operator RGBColor() const 
	{
		Float32 r, g, b;

		Float32 h = m_hue % 360.0f;

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

		if(m_lum <= 0.5f)
        {
            r *= (m_lum * 2.0f);
            g *= (m_lum * 2.0f);
            b *= (m_lum * 2.0f);
        }
        else
        {
            Float32 value = (m_lum - 0.5f) * 2.0f;
            r += value - (value * r);
            g += value - (value * g);
            b += value - (value * b);
        }

        r = (r * m_sat) + (m_lum * (1.0f - m_sat));
        g = (g * m_sat) + (m_lum * (1.0f - m_sat));
        b = (b * m_sat) + (m_lum * (1.0f - m_sat));

		return RGBColor(r, g, b);
	}

	Float32 GetHue()        const { return m_hue; }
	Float32 GetSaturation() const { return m_sat; }
	Float32 GetLuminance()  const { return m_lum; }

	void SetHue       (Float32 hue       ) { m_hue = hue;        }
	void SetSaturation(Float32 saturation) { m_sat = saturation; }
	void SetLuminance (Float32 luminance ) { m_lum = luminance;  }
};