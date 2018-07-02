#include <iostream>
#include "pmlib/pmlib.h"
using namespace std;

void leaf(vec3 origin, vec3 direction, float width, float height, int res)
{
	direction.y = 0;
	direction = direction.normalized();
	vec3 localRight = direction * u;
	localRight = localRight.normalized();


}

int main()
{

}