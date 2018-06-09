#include <math.h>
#define pi 3.14159265

struct vector
{
    float x;
    float y;
    float z;

    vector()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    vector(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // i j k
    // x y z
    // o p q

    // iyq + kxp + jzo - izp - jxq - kyo
    vector cross(vector other)
    {
        vector res;
        res.x = y * other.z - z * other.y;
        res.y = z * other.x - x * other.z;
        res.z = x * other.y - y * other.x;
        return res;
    }
    float magnitude()
    {
        return sqrt(x*x + y*y + z*z);
    }
    vector normalized()
    {
        vector res;
        float mag = magnitude();
        res.x = x/mag;
        res.y = y/mag;
        res.z = z/mag;
        return res;
    }
};

vector r(1,0,0);
vector u(0,1,0);
vector b(0,0,1);

// vector addition
vector operator+(const vector& a, const vector& b)
{
    vector res(a.x + b.x, a.y + b.y, a.z + b.z);
    return res;
}
void operator+=(vector& a, const vector& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}
// vector subtraction
vector operator-(const vector& a, const vector& b)
{
    vector res(a.x - b.x, a.y - b.y, a.z - b.z);
    return res;
}
void operator-=(vector& a, const vector& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}
// scalar product
vector operator*(const vector& a, const float& factor)
{
    vector res(a.x * factor, a.y * factor, a.z * factor);
    return res;
}
// cross product
vector operator*(const vector& a, const vector& b)
{
    vector res(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    return res;
}
// times -1
vector operator-(const vector& v)
{
    vector res(v.x * -1, v.y * -1, v.z * -1);
    return res;
}
// for printing
std::ostream& operator<<(std::ostream& os, const vector& v)
{
    return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}