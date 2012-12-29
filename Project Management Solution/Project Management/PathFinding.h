#pragma once
#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <vector>
#include <array>
#include <map>
using std::array;
using std::vector;
using std::map;

#include "House.h"

class PathFinding
{
public:
	PathFinding(void);
	~PathFinding(void);
	void addHouse(House *house);
	void update();
	vector<array<float, 3>> getPath(array<float, 3> a, array<float, 3> b);
	
private:
	struct node
	{
		array<float,3> pos;
		vector<int> available;
	};
	vector<node> nodes;
	vector<vector<node>> paths;
	map<array<array<float,3>, 2>, vector<array<float, 3>>> storedPaths;

	float cF(float a, float b, bool _largest);
	void findAvailable(vector<array<float, 3>> nodes, int index, vector<int> &available);
	void calculatePath(array<float, 3> a, array<float, 3> b);
	void nextNode(node n, vector<node> visited, node goalNode);
	vector<House *> building;
};

