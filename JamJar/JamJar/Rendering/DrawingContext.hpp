#pragma once

#include "../Math/Vector.hpp"

class DrawingContext
{
public:
	virtual void DrawRectangle     (Vector2F32 position, Vector2F32 scale) = 0;
	virtual void DrawEllipse       (Vector2F32 position, Vector2F32 scale) = 0;
	virtual void DrawRegularPolygon(Vector2F32 position, Vector2F32 scale, UInt32 vertexCount, Float32 angle) = 0;
};