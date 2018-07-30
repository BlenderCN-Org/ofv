#include <fstream>
#include <string>
#include <stdlib.h>     /* atof */
#include <iostream>
using namespace std;

float currentParameterSensitivity;
float currentParameterMin;
float currentParameterMax;

char currentParameterType;

bool stringMatchesString(string a, string b, int position)
{
  for (int i = 0; i < b.length(); i++)
  {
    if (position + i >= a.length() || a.at(position + i) != b.at(i))
    {
      return false;
    }
  }
  return true;
}

bool stringContainsChar(string theString, char theChar)
{
	for (int i = 0; i < theString.length(); i++)
	{
		if (theString[i] == theChar)
			return true;
	}
	return false;
}

string getParameterName(string fC, int i)
{
	string res;
	while (fC[i] != ' ')
	{
		i++;
	}
	i++;
	while (fC[i] != ' ' && fC[i] != '=' && fC[i] != ';')
	{
		res += fC[i];
		i++;
	}
	return res;
}

bool checkForGlobalVariable(string fC, int i)
{
	if (stringMatchesString(fC, "float", i))
	{
		currentParameterType = 'f';
		return true;
	}
	else if (stringMatchesString(fC, "int", i))
	{
		currentParameterType = 'i';
		return true;
	}
	else if (stringMatchesString(fC, "bool", i))
	{
		currentParameterType = 'b';
		return true;
	}
	return false;
}

void getParameterValues(string fC, int i)
{
	currentParameterMax = 0;
	currentParameterMin = 0;
	currentParameterSensitivity = 0.1;

	while (fC[i] != ';')
		i++;

	while (fC[i] != '\n')
	{
		if (stringContainsChar("0123456789[", fC[i]))
		{
			string helper = "";
			if (fC[i] == '[')
			{
				i++;
				while (fC[i] != ',')
				{
					helper += fC[i];
					i++;
				}

				cout << "min" << helper << endl;
				currentParameterMin = atof(helper.c_str());

				i++;
				helper = "";
				while (fC[i] != ']')
				{
					helper += fC[i];
					i++;
				}

				cout << "max" << helper << endl;
				currentParameterMax = atof(helper.c_str());
			}
			else
			{
				while (stringContainsChar("0123456789.", fC[i]))
				{
					helper += fC[i];
					i++;
				}
				i--;
				cout << "sens" << helper << endl;
				currentParameterSensitivity = atof(helper.c_str());
			}
		}
		i++;
	}
	cout << "leaving" << endl;
}

int main(int argc, char** argv)
{
	ifstream ifs (argv[1], ifstream::in);
	string fileContents((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

	ofstream modelFile;
	modelFile.open("cppmodel.h");
	modelFile << fileContents;
	modelFile.close();

	ofstream binderFile;
	binderFile.open("binder.h");
	binderFile << "#include <vector>\n\nvoid bindParameters()\n{\n";

	for (int i = 0; i < fileContents.length(); i++)
	{
		if (checkForGlobalVariable(fileContents, i))
		{
			string name = getParameterName(fileContents, i);
			if (stringContainsChar(name, '('))
			{
				continue;
			}

			getParameterValues(fileContents, i);

			binderFile << "\tparametersVector.push_back(parameter(\"" << name << "\", '" << currentParameterType << "', &" << name << ", " << currentParameterSensitivity << ", " << currentParameterMin << ", " << currentParameterMax << "));\n";
		}
		else if (stringMatchesString(fileContents, "(", i))
		{
			binderFile << "}\n";
			binderFile.close();
			return 0;
		}
	}
}