#include <vector>

struct parameter
{
	string name;
	char type;
	void* variable;
};

int currentParameter = 0;

std::vector<parameter> parameters;

void bindParameters()
{
	parameter radiusParameter;
	parameter heightParameter;
	parameter hResolutionParameter;
	parameter vResolutionParameter;
	parameter sidesParameter;

	radiusParameter.name = "radius";
	heightParameter.name = "height";
	hResolutionParameter.name = "hResolutionParameter";
	vResolutionParameter.name = "vResolutionParameter";
	sidesParameter.name = "sides";

	radiusParameter.type = 'f';
	heightParameter.type = 'f';
	hResolutionParameter.type = 'i';
	vResolutionParameter.type = 'i';
	sidesParameter.type = 'i';

	radiusParameter.variable = &radius;
	heightParameter.variable = &height;
	hResolutionParameter.variable = &hResolution;
	vResolutionParameter.variable = &vResolution;
	sidesParameter.variable = &sides;

	parameters.push_back(radiusParameter);
	parameters.push_back(heightParameter);
	parameters.push_back(hResolutionParameter);
	parameters.push_back(vResolutionParameter);
	parameters.push_back(sidesParameter);
}