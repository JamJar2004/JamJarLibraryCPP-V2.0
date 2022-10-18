#pragma once

class GraphicsMode
{
private:
	UInt32 m_width;
	UInt32 m_height;
public:
	GraphicsMode(UInt32 width, UInt32 height) {}

	UInt32 GetWidth()  const { return m_width; }
	UInt32 GetHeight() const { return m_height; }
};

class Application
{
public:
	virtual void Start(GraphicsMode graphicsMode) = 0;
	virtual void Exit() = 0;
};