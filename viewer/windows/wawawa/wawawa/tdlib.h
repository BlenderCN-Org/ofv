#include <math.h>
#define pi 3.14159265358979

struct vec3
{
	float x;
	float y;
	float z;

	vec3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// i j k
	// x y z
	// o p q

	// iyq + kxp + jzo - izp - jxq - kyo
	vec3 cross(vec3 other)
	{
		vec3 res;
		res.x = y * other.z - z * other.y;
		res.y = z * other.x - x * other.z;
		res.z = x * other.y - y * other.x;
		return res;
	}
	float magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}
	vec3 normalized()
	{
		vec3 res;
		float mag = magnitude();
		res.x = x / mag;
		res.y = y / mag;
		res.z = z / mag;
		return res;
	}
};

const vec3 r(1, 0, 0);
const vec3 u(0, 1, 0);
const vec3 b(0, 0, 1);

// vector addition
vec3 operator+(const vec3& a, const vec3& b)
{
	vec3 res(a.x + b.x, a.y + b.y, a.z + b.z);
	return res;
}
void operator+=(vec3& a, const vec3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}
// vector subtraction
vec3 operator-(const vec3& a, const vec3& b)
{
	vec3 res(a.x - b.x, a.y - b.y, a.z - b.z);
	return res;
}
void operator-=(vec3& a, const vec3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
}
// scalar product
vec3 operator*(const vec3& a, const float& factor)
{
	vec3 res(a.x * factor, a.y * factor, a.z * factor);
	return res;
}
// cross product
vec3 operator*(const vec3& a, const vec3& b)
{
	vec3 res(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	return res;
}
// times -1
vec3 operator-(const vec3& v)
{
	vec3 res(v.x * -1, v.y * -1, v.z * -1);
	return res;
}