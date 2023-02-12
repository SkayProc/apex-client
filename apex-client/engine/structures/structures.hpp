#pragma once

#include <algorithm>

namespace engine::structures
{
	enum class bones_t : std::uint16_t
	{
		head = 0,


	};

	struct matrix3x4_t
	{
		union
		{
			float matrix3x4[3][4];
		};
	};

	struct matrix4x4_t
	{
		union
		{
			float matrix4x4[4][4];
		};
	};

	class vector3_t
	{
	public:
		float x; //0x0000 
		float y; //0x0004 
		float z; //0x0008 

		vector3_t() { x = 0; y = 0; z = 0; }
		vector3_t(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }
		void Clamp()
		{
			x = std::clamp(remainderf(x, 360.0f), -89.0f, 89.0f);
			y = std::clamp(remainderf(y, 360.0f), -180.0f, 180.0f);
		}

		auto Clamped() {
			vector3_t tmp = *this;
			tmp.Clamp();
			return tmp;
		}

		void Normalize()
		{
			if (x > 180)
				x -= 360.0f;
			if (x < 180)
				x += 360.0f;
			if (y > 180)
				y -= 360.0f;
			if (y < 180)
				y += 360.0f;
		}

		vector3_t Normalized() {
			vector3_t tmp = *this;
			tmp.Normalize();
			return tmp;
		}

		vector3_t operator-(vector3_t angle)
		{
			return vector3_t(x - angle.x, y - angle.y, z - angle.z);
		}

		void operator-=(vector3_t angle)
		{
			x -= angle.x;
			y -= angle.y;
			z -= angle.z;
		}


		vector3_t operator+(vector3_t angle)
		{
			return vector3_t(x + angle.x, y + angle.y, z + angle.z);
		}

		vector3_t& operator+=(const vector3_t& vec)
		{
			return (*this = *this + vec);
		}

		vector3_t operator*(float f)
		{
			return vector3_t(x * f, y * f, z * f);
		}

		vector3_t operator/(vector3_t angle)
		{
			return vector3_t(x / angle.x, y / angle.y, z / angle.z);
		}

		vector3_t operator/(const float& f)
		{
			return vector3_t(x / f, y / f, z / f);
		}

		vector3_t operator/=(float& f)
		{
			return vector3_t(x / f, y / f, z / f);
		}

		float& operator[](int i) const
		{
			return ((float*)this)[i];
		}

		bool operator==(vector3_t v)
		{
			return (v.x == x && v.y == y && v.z == z);
		}

		bool operator!=(vector3_t v)
		{
			return(v.x != x || v.y != y || v.z != z);
		}

		friend vector3_t operator* (float lhs, vector3_t rhs) {
			return rhs * lhs;
		}

		bool IsNan()
		{
			if (isnan(x) || isnan(y) || isnan(z))
				return true;

			return false;
		}

		auto LengthSqr() const {
			return (x * x + y * y + z * z);
		}

		auto Length2DSqr() const {
			return (x * x + y * y);
		}

		auto Length() const {
			return sqrt(LengthSqr());
		}

		auto Length2D() const {
			return sqrt(Length2DSqr());
		}

		constexpr auto notNull() const noexcept
		{
			return x || y || z;
		}

		bool isNull()
		{
			return (x < 1 && y < 1 && z < 1);
		}

		//to RGB decimals to ints
		vector3_t ToRGB(vector3_t& in)
		{
			return in = in * 255;
		}

		//to RGB ints to decimals
		vector3_t ToDRGB(vector3_t& in)
		{
			return in = in / 255;
		}

		vector3_t FloatToColor(float* color)
		{
			return vector3_t(color[0], color[1], color[2]);
		}

		float dot_product(const vector3_t& vec)
		{
			return (this->x * vec.x + this->y * vec.y + this->z * vec.z);
		}

		const float dot(const vector3_t& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		float dist_to(const vector3_t& vec)
		{
			vector3_t delta;

			delta.x = this->x - vec.x;
			delta.y = this->y - vec.y;
			delta.z = this->z - vec.z;

			return delta.Length();
		}
	};

	struct vector2_t
	{
		float x;
		float y;

		vector2_t operator-(vector2_t value)
		{
			return { x - value.x, y - value.y };
		}

		vector2_t operator+(vector2_t value)
		{
			return { x + value.x, y + value.y };
		}

		vector2_t operator*(float value)
		{
			return { x * value, y * value };
		}

		vector2_t operator/(float value)
		{
			return { x / value, y / value };
		}

		vector2_t operator/=(float value)
		{
			x /= value;
			y /= value;

			return *this;
		}

		vector2_t operator+=(vector2_t value)
		{
			return { x += value.x, y += value.y };
		}

		vector2_t operator-=(vector2_t value)
		{
			return { x -= value.x, y -= value.y };
		}

		const float dot(const vector2_t& vec) const
		{
			return x * vec.x + y * vec.y;
		}
	};

	struct vector4_t
	{
		float x;
		float y;
		float z;
		float w;

		float color[4] = { x, y, z, w };

		const float dot(const vector3_t& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}

		vector3_t operator-(const vector3_t& vec) const
		{
			return { x - vec.x, y - vec.y, z - vec.z };
		}
	};

	union view_matrix_t
	{
		struct
		{
			const float        _11, _12, _13, _14;
			const float        _21, _22, _23, _24;
			const float        _31, _32, _33, _34;
			const float        _41, _42, _43, _44;

		};

		const float m[4][4];
	};
}