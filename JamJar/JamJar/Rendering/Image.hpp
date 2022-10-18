#pragma once

#include "../Numerics.hpp"

class Image
{
	virtual UInt32 GetWidth()  const = 0;
	virtual UInt32 GetHeight() const = 0;
};