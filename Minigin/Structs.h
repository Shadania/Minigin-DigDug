#pragma once


namespace dae
{
	struct Float2
	{
		float x{}, y{};
		Float2& operator+=(const Float2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Float2& operator-=(const Float2& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Float2& operator*=(const Float2& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
	};

	struct Float3
	{
		float x{}, y{}, z{};
	};

	struct Float4
	{
		float x{}, y{}, z{}, w{};
	};
}