#pragma once
#include "Structs.h"

namespace dae
{
	namespace utils
	{
		bool DoRectsOverlap(const Float4& a, const Float4& b)
		{
			return !((a.x > (b.x + b.z)) ||
				((a.x + a.z) < b.x) ||
				(a.y > (b.y + b.w)) ||
				((a.y + a.z) < b.y));
		}
	}
}