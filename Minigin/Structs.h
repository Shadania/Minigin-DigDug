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
		Float2& operator/=(float other)
		{
			x /= other;
			y /= other;
			return *this;
		}

		Float2 operator*(float other) const
		{
			Float2 obj{*this};
			obj.x *= other;
			obj.y *= other;
			return obj;
		}
		Float2 operator-(const Float2& other) const
		{
			Float2 obj{*this};
			obj.x -= other.x;
			obj.y -= other.y;
			return obj;
		}
		Float2 operator+(const Float2& other) const
		{
			Float2 obj{ *this };
			obj.x += other.x;
			obj.y += other.y;
			return obj;
		}

		float Length(const Float2& vec) const
		{
			return (sqrt(vec.x * vec.x + vec.y * vec.y));
		}
		float LengthSqr(const Float2& vec) const
		{
			return (vec.x * vec.x + vec.y * vec.y);
		}
		bool IsWithinDistFromPos(float dist, const Float2& pos) const
		{
			return (LengthSqr(pos - *this) <= (dist*dist));
		}
		Float2() = default;
		Float2(float x, float y)
			:x{x}
			,y{y}
		{}
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
		Float4(const Float3& xyz, float w)
			:Float4(xyz.x, xyz.y, xyz.z, w)
		{}
		Float4(const Float2& xy, const Float2& zw)
			:Float4(xy.x, xy.y, zw.x, zw.y)
		{}


		float x{}, y{}, z{}, w{};
	};
}