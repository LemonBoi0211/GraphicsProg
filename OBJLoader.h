#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "Vertex.h"
#include <map>

using namespace std;
using namespace glm;

class OBJLoader
{
public:
	static vector<Vertex> LoadOBJ(const string& FolderLoc,
		const string& Filename, string& AmbiantLoc, string& Diffloc,
		string& specLoc, string& NormalLoc, vector<uint>& indices);

	static void LoadMaterial(const string& MatLibLoc, string& AmbiantLoc,
		string& DiffLoc, string& specLoc, string& NormalLoc);

};

#endif // !OBJLOADER_H