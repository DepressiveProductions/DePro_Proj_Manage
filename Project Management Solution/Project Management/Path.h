#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <vector>
#include <array>
#include <iostream>
using std::array;
using std::vector;

#include "House.h"

class Path
{
public:
	Path(void);
	~Path(void);

	void addBuildingType(House *house);
	void update();

	void draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack);

private:
	vector<House *> buildingTypes;
	struct node
	{
		array<float, 3> pos;
		vector<node *> available;
	};
	vector<node> nodes;

	void copyNodes();
	void setAvailable();

	bool pValid(array<float,3> p1, array<float,3> p2, array<float,4> walls); //Quick filter - true if valid path
	bool pInvalid(array<float,3> p1, array<float,3> p2, array<float,4> walls); //Quick filter - true if invalid path
	bool lineCheck(array<float,3> p1, array<float,3> p2, array<float,4> walls); //Mathematic filter - true if valid path
	float applyLine(float v1, float v2, float wall, float cVar1, float cVar2); //Mathematical operations with point, line and wall
};

