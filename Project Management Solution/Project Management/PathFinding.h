#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <vector>
#include <array>
using std::array;
using std::vector;

#include "House.h"

class PathFinding
{
public:
	PathFinding(void);
	~PathFinding(void);
	void init(House &house);
	void update();

private:
	/*struct node
	{
		array<float,3> pos;
		vector<int> availNodeIDs;
	};
	vector<node> nodes;*/

	void findAvailableNodes(array<float,3> pos, vector<int> &available);
	void calculatePaths();
	House buildings;
};

