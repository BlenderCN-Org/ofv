#include <vector>

void bindParameters()
{
	parametersVector.push_back(parameter("seed", 'i', &seed, 0.1, 0, 0));
	parametersVector.push_back(parameter("count", 'i', &count, 0.1, 3, 30));
	parametersVector.push_back(parameter("petals", 'i', &petals, 0.1, 1, 20));
	parametersVector.push_back(parameter("petalWidth", 'f', &petalWidth, 0.05, 0, 0));
	parametersVector.push_back(parameter("petalLength", 'f', &petalLength, 0.05, 0, 0));
	parametersVector.push_back(parameter("petalHeight", 'f', &petalHeight, 0.05, 0, 0));
	parametersVector.push_back(parameter("petalResZ", 'i', &petalResZ, 0.1, 0, 0));
	parametersVector.push_back(parameter("petalResX", 'i', &petalResX, 0.1, 0, 0));
	parametersVector.push_back(parameter("radius", 'f', &radius, 0.005, 0, 0));
	parametersVector.push_back(parameter("ballResH", 'i', &ballResH, 0.1, 0, 0));
	parametersVector.push_back(parameter("ballResV", 'i', &ballResV, 0.1, 0, 0));
	parametersVector.push_back(parameter("ballHeight", 'f', &ballHeight, 0.1, 0, 0));
	parametersVector.push_back(parameter("ballOffset", 'f', &ballOffset, 0.1, 0, 0));
	parametersVector.push_back(parameter("ballMaxRadiusMultiplier", 'f', &ballMaxRadiusMultiplier, 0.05, 0, 0));
	parametersVector.push_back(parameter("ballMinRadiusMultiplier", 'f', &ballMinRadiusMultiplier, 0.05, 0, 0));
	parametersVector.push_back(parameter("minPetalAngle", 'f', &minPetalAngle, 0.05, 0, 0));
	parametersVector.push_back(parameter("maxPetalAngle", 'f', &maxPetalAngle, 0.05, 0, 0));
}
