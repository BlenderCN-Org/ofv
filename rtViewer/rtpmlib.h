#include "tdlib.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int vertexCounter = 0;

struct vertexS
{
    string id;
    vec3 pos;
};
struct faceS
{
    vector<vertexS> verts;
    vec3 normal;
};

map <int, vertexS> mapping;
vector<faceS> faces;

void clearModel()
{
    mapping.clear();
    faces.clear();
}

void calcNormal(faceS& theFace)
{
    vec3 a = theFace.verts[1].pos - theFace.verts[0].pos;
    vec3 b = theFace.verts[2].pos - theFace.verts[1].pos;

    theFace.normal = a * b;
    theFace.normal = theFace.normal.normalized();
}

int vertex(float x, float y, float z)
{
    vertexS vtx;
    vtx.id =  vertexCounter;
    vtx.pos.x = x;
    vtx.pos.y = y;
    vtx.pos.z = z;

    mapping.insert(pair<int, vertexS>(vertexCounter, vtx));

    vertexCounter++;
    return vertexCounter - 1;
}
int vertex(vec3 pos)
{
    vertexS vtx;
    vtx.id =  vertexCounter;
    vtx.pos.x = pos.x;
    vtx.pos.y = pos.y;
    vtx.pos.z = pos.z;

    mapping.insert(pair<int, vertexS>(vertexCounter, vtx));

    vertexCounter++;
    return vertexCounter - 1;
}

void face(int* ids, int length)
{
    faceS fce;
    for (int i = 0; i < length; i++)
    {
        fce.verts.push_back(mapping[ids[i]]);
    }
    calcNormal(fce);
    faces.push_back(fce);
}
void face(int* ids, int length, bool invert)
{
    faceS fce;
    if (invert)
    {
        for (int i = length - 1; i > -1; i--)
        {
            fce.verts.push_back(mapping[ids[i]]);
        }
        calcNormal(fce);
        faces.push_back(fce);
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            fce.verts.push_back(mapping[ids[i]]);
        }
        calcNormal(fce);
        faces.push_back(fce);
    }
}
void face(int* ids, int length, int start)
{
    faceS fce;
    for (int i = 0; i < length; i++)
    {
        fce.verts.push_back(mapping[ids[i + start]]);
    }
    calcNormal(fce);
    faces.push_back(fce);
}
void face(int* ids, int length, int start, bool invert)
{
    faceS fce;
    if (invert)
    {
        for (int i = length - 1; i > -1; i--)
        {
            fce.verts.push_back(mapping[ids[i + start]]);
        }
        calcNormal(fce);
        faces.push_back(fce);
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            fce.verts.push_back(mapping[ids[i + start]]);
        }
        calcNormal(fce);
        faces.push_back(fce);
    }
}
void faceSeq(int* ids, int count, int vertsPerFace)
{
    faceS fce[count / vertsPerFace];

    int curFce = 0;

    for (int i = 0; i < count; i++)
    {
        fce[curFce].verts.push_back(mapping[ids[i]]);
        if ((i + 1) % vertsPerFace == 0)
        {
            calcNormal(fce[curFce]);
            faces.push_back(fce[curFce]);
            curFce++;
        }
    }
}

void closeFile()
{
    //output.close();
}
