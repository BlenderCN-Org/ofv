#include <fstream>
#include <string>
using namespace std;

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
		if (stringMatchesString(fileContents, "float", i))
		{
			string name = getParameterName(fileContents, i);
			if (stringContainsChar(name, '('))
			{
				continue;
			}
			binderFile << "\tparametersVector.push_back(parameter(\"" << name << "\", 'f', &" << name << "));\n";
		}
		else if (stringMatchesString(fileContents, "int", i))
		{
			string name = getParameterName(fileContents, i);
			if (stringContainsChar(name, '('))
			{
				continue;
			}
			binderFile << "\tparametersVector.push_back(parameter(\"" << name << "\", 'i', &" << name << "));\n";
		}
		else if (stringMatchesString(fileContents, "bool", i))
		{
			string name = getParameterName(fileContents, i);
			if (stringContainsChar(name, '('))
			{
				continue;
			}
			binderFile << "\tparametersVector.push_back(parameter(\"" << name << "\", 'b', &" << name << "));\n";
		}
		else if (stringMatchesString(fileContents, "(", i))
		{
			binderFile << "}\n";
			binderFile.close();
			return 0;
		}
	}
}