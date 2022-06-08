#include <JamJar/Core.hpp>

//#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

class Shape
{
public:
	virtual Float64 GetArea() const = 0;
};

class Rectangle : public Shape
{
private:
	Float64 m_width;
	Float64 m_height;
public:
	Rectangle(Float64 width, Float64 height) : m_width(width), m_height(height) {}

	Float64 GetWidth()  const { return m_width;  }
	Float64 GetHeight() const { return m_height; }

	virtual Float64 GetArea() const override { return m_width * m_height; }
};

class Circle : public Shape
{
private:
	Float64 m_radius;
public:
	Circle(Float64 radius) : m_radius(radius) {}

	Float64 GetRadius() const { return m_radius; }

	Float64 GetDiameter() const { return m_radius * 2.0; }

	virtual Float64 GetArea() const override { return Float64::PI * m_radius * m_radius; }
};

ExitStatus Start()
{
	//ArrayList<String> list;
	//list.Add("ABC");
	//list.Add("DEF");
	//list.Add("GHI");
	//list.Add("JKL");
	//list.Add("MNO");
	//list.Add("PQR");

	//Console::PrintLine(list);

	NullableRef<Rectangle> shape = nullptr;

	Console::PrintLine(shape->GetArea());

	return ExitStatus::OK;
}