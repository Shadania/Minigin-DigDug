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
		Float3(float x, float y, float z)
			:x{x}
			,y{y}
			,z{z}
		{}

		float x{}, y{}, z{};
	};

	struct Float4
	{
		Float4(float x, float y, float z, float w)
			: x{ x }
			, y{ y }
			, z{ z }
			, w{ w }
		{}
		Float4()
			:x{0}
			,y{0}
			,z{0}
			,w{0}
		{}
		Float4(const Float2& xy, float z, float w)
			:Float4(xy.x, xy.y, z, w)
		{}


		float x{}, y{}, z{}, w{};
	};
}