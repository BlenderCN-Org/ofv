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

string getParameterName(string fC, int i)
{
	string res;
	while (fC[i] != ' ')
	{
		i++;
	}
	i++;
	while (fC[i] != ' ' && fC[i] != '=')
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

	ofstream output;
	output.open("binder.h");
	output << "#include <vector>\n\nvoid bindParameters()\n{\n";

	for (int i = 0; i < fileContents.length(); i++)
	{
		if (stringMatchesString(fileContents, "float", i))
		{
			string name = getParameterName(fileContents, i);
			output << "\tparameter " << name << "Parameter;\n";
			output << '\t' << name << "Parameter.name = \"" << name << "\";\n";
			output << '\t' << name << "Parameter.type = 'f';\n";
			output << '\t' << name << "Parameter.variable = &" << name << ";\n";
			output << '\t' << "parameters.push_back(" << name << "Parameter);\n";
		}
		else if (stringMatchesString(fileContents, "int", i))
		{
			string name = getParameterName(fileContents, i);
			output << "\tparameter " << name << "Parameter;\n";
			output << '\t' << name << "Parameter.name = \"" << name << "\";\n";
			output << '\t' << name << "Parameter.type = 'i';\n";
			output << '\t' << name << "Parameter.variable = &" << name << ";\n";
			output << '\t' << "parameters.push_back(" << name << "Parameter);\n";
		}
		else if (stringMatchesString(fileContents, "void generateModel()", i))
		{
			output << "}\n";
			return 0;
		}
	}
}