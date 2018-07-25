#include <vector>

void bindParameters()
{
	parameter radiusParameter;
	radiusParameter.name = "radius";
	radiusParameter.type = 'f';
	radiusParameter.variable = &radius;
	parameters.push_back(radiusParameter);
	parameter heightParameter;
	heightParameter.name = "height";
	heightParameter.type = 'f';
	heightParameter.variable = &height;
	parameters.push_back(heightParameter);
	parameter hResolutionParameter;
	hResolutionParameter.name = "hResolution";
	hResolutionParameter.type = 'i';
	hResolutionParameter.variable = &hResolution;
	parameters.push_back(hResolutionParameter);
	parameter vResolutionParameter;
	vResolutionParameter.name = "vResolution";
	vResolutionParameter.type = 'i';
	vResolutionParameter.variable = &vResolution;
	parameters.push_back(vResolutionParameter);
	parameter sidesParameter;
	sidesParameter.name = "sides";
	sidesParameter.type = 'i';
	sidesParameter.variable = &sides;
	parameters.push_back(sidesParameter);
}
