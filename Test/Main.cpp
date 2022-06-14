#include <JamJar/Core.hpp>

#include <JamJar/Data/Collections/Lists/ArrayList.hpp>

#include <JamJar/Data/Memory/Stack.hpp>
#include <JamJar/Data/Memory/Queue.hpp>

#include <JamJar/Dynamic.hpp>

#include <JamJar/Data/Collections/Maps/HashMap.hpp>

#include <JamJar/Math/Vector.hpp>

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

class Object
{
private:
public:
	Object() { Console::PrintLine("Created!"); }

	Object(const Object& other) { Console::PrintLine("Copied!"); }

	Object(Object&& other) noexcept { Console::PrintLine("Moved"); }

	~Object() { Console::PrintLine("Destroyed!"); }

	Object& operator=(const Object& other) { Console::PrintLine("Assigned!"); return *this; }
};

ExitStatus Start()
{
	/*HashMap<String, UInt32> map;
	map.Add("A", 1U);
	map.Add("B", 2U);
	map.Add("C", 3U);
	map.Add("D", 4U);
	map.Add("E", 5U);
	map.Add("G", 7U);
	map.Add("F", 6U);
	map.Add("H", 8U);
	map.Add("I", 9U);
	map.Add("J", 10U);
	map.Add("K", 11U);
	map.Add("L", 12U);
	map.Add("M", 13U);
	map.Add("N", 14U);
	map.Add("O", 15U);
	map.Add("P", 16U);
	map.Add("Q", 17U);
	map.Add("R", 18U);
	map.Add("S", 19U);

	Console::PrintLine(map);*/



	return ExitStatus::OK;
}