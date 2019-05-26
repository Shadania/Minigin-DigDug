#include "MiniginPCH.h"
#include "utils.h"


bool dae::utils::DoRectsOverlap(const Float4& a, const Float4& b)
{
	return !((a.x > (b.x + b.z)) ||
		((a.x + a.z) < b.x) ||
		(a.y > (b.y + b.w)) ||
		((a.y + a.z) < b.y));
}

bool dae::utils::IsPointInRect(const Float4& rect, const Float2& pos)
{
	if (pos.x > rect.x)
		if (pos.x < (rect.x + rect.z))
			if (pos.y > rect.y)
				if (pos.y < (rect.y + rect.w))
					return true;
	return false;
}