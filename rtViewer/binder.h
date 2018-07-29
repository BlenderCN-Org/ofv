#include <vector>

void bindParameters()
{
	parametersVector.push_back(parameter("stepCount", 'i', &stepCount));
	parametersVector.push_back(parameter("stepSize", 'f', &stepSize));
	parametersVector.push_back(parameter("stepHeight", 'f', &stepHeight));
	parametersVector.push_back(parameter("stepAngle", 'f', &stepAngle));
	parametersVector.push_back(parameter("thickness", 'f', &thickness));
	parametersVector.push_back(parameter("createRailings", 'b', &createRailings));
	parametersVector.push_back(parameter("createSideCap", 'b', &createSideCap));
	parametersVector.push_back(parameter("railingHOffset", 'f', &railingHOffset));
	parametersVector.push_back(parameter("railingVOffset", 'f', &railingVOffset));
	parametersVector.push_back(parameter("railingWidth", 'f', &railingWidth));
	parametersVector.push_back(parameter("railingThickness", 'f', &railingThickness));
	parametersVector.push_back(parameter("railingPipeEachXSteps", 'i', &railingPipeEachXSteps));
	parametersVector.push_back(parameter("railingPipeHeight", 'f', &railingPipeHeight));
	parametersVector.push_back(parameter("railingPipeRes", 'i', &railingPipeRes));
	parametersVector.push_back(parameter("railingPipeRadius", 'f', &railingPipeRadius));
	parametersVector.push_back(parameter("railingPipeTwistAngle", 'f', &railingPipeTwistAngle));
	parametersVector.push_back(parameter("railingPipePolygonSides", 'i', &railingPipePolygonSides));
}
