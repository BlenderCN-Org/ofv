#include "tdlib.h"
#include <fstream>
#include <string>
using namespace std;

int vertexCounter = 0;
bool fileOpen = false;
bool objectCreated = false;
ofstream output;

void checkObjectAndFile()
{
    if (!fileOpen)
    {
        output.open("output.ofv");
        fileOpen = true;
    }
    if (!objectCreated)
    {
        output << "o[object]\n";
        objectCreated = true;
    }
}

int vertex(float x, float y, float z)
{
    checkObjectAndFile();
    output << 'v' << vertexCounter << '[' << x << ',' << y << ',' << z << "]\n";
    vertexCounter++;
    return vertexCounter - 1;
}
int vertex(vec3 pos)
{
    checkObjectAndFile();
    output << 'v' << vertexCounter << '[' << pos.x << ',' << pos.y << ',' << pos.z << "]\n";
    vertexCounter++;
    return vertexCounter - 1;
}

void face(int* ids, int length)
{
    output << "f[";
    for (int i = 0; i < length; i++)
    {
        output << 'v' << ids[i];
        if (i == length - 1)
        {
            output << "]\n";
        }
        else
        {
            output << ',';
        }
    }
}

void closeFile()
{
    output.close();
}
