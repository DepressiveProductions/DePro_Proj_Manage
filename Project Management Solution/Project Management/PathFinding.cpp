#include "PathFinding.h"

PathFinding::PathFinding() {}
PathFinding::~PathFinding() {}

void PathFinding::addHouse(House *house)
{
	building.push_back(house);
}

void PathFinding::update()
{
	vector<array<float, 3>> pathNodes;
	for (unsigned int i=0; i<building.size(); i++)
	{
		building[i]->getNodes(pathNodes);
	}

	for (unsigned int i=0; i<pathNodes.size(); i++)
	{
		vector<int> aIdx; // Available node's indexes
		findAvailable(pathNodes, i, aIdx);
		node n;
		n.pos = pathNodes[i];
		n.availNodeIDs = aIdx;
		nodes.push_back(n);
	}
}

float PathFinding::cF(float a, float b, bool _largest) // Compare floats, returns largest if _largest is true
{
	if (_largest && a >= b)
		return a;
	else if (!_largest && a >= b)
		return b;
	else if (_largest && a < b)
		return b;
	else if (!_largest && a < b)
		return a;
}

void PathFinding::findAvailable(vector<array<float, 3>> nodes, int index, vector<int> &available)
{
	for (unsigned int i=0; i<nodes.size(); i++)
	{
		if (index != i)
		{
			// y=kx+m equation to check if the line between the two points doesn't cross a house-wall
			float y, k, x, m; // nodes[index] contains x and y
			if (nodes[i][0] != nodes[index][0]) // Prevent divide by zero
			{
				k = (nodes[index][1]-nodes[i][1])/(nodes[index][0]-nodes[i][0]);
				m = nodes[index][1]-(k*nodes[index][0]);
				
				// Check if line crosses a wall
				for (unsigned int j=0; j<building.size(); j++)
				{
					for (unsigned int jj=0; jj<building[j]->getHouses().size(); jj++)
					{
						array<float, 4> walls = building[j]->getHouses()[jj]->getWalls();
						// Check that the 2 points are on each side of the building
						if (!(cF(nodes[index][0], nodes[i][0], true) < walls[0] || cF(nodes[index][0], nodes[i][0], false) > walls[1] || cF(nodes[index][1], nodes[i][1], true) > walls[3] || cF(nodes[index][1], nodes[i][1], false) < walls[2]))
						{
							// Check if it crosses either wall
							if (!(((k*walls[0])+m <= walls[3] && (k*walls[0])+m >= walls[2]) || // Crosses left wall
								((k*walls[1])+m <= walls[3] && (k*walls[1])+m >= walls[2]) || // Crosses right wall
								((walls[2]-m)/k <= walls[0] && (walls[2]-m)/k <= walls[1]) || // Crosses lower wall
								((walls[3]-m)/k <= walls[0] && (walls[3]-m)/k <= walls[1]))) // Crosses upper wall
							{
								available.push_back(i);
							}
						}
						else
							available.push_back(i);
					}
				}
			}
			else // Both x's have the same value
			{
				for (unsigned int j=0; j<building.size(); j++)
				{
					for (unsigned int jj=0; jj<building[j]->getHouses().size(); jj++)
					{
						array<float, 4> walls = building[j]->getHouses()[jj]->getWalls();
						// Check that atleast 1 point is inside/on the other side of the building
						if (!(cF(nodes[index][1], nodes[i][1], false) > walls[3] || cF(nodes[index][1], nodes[i][1], true) < walls[2]))
						{
							// Chack if this path goes through any wall
							if (!(nodes[i][0] <= walls[1] && nodes[i][0] >= walls[0]))
								available.push_back(i);
						}
						else
							available.push_back(i);
					}
				}
			}
		}
	}
}

vector<array<float, 3>> PathFinding::getPath(array<float, 3> a, array<float, 3> b)
{
	array<array<float,3>, 2> startToGoal;
	// Check if path already is stored
	for (unsigned int i=0; i<storedPaths.size(); i++)
	{
		if (storedPaths.find(startToGoal) != storedPaths.end()) // Check that start_goal is stored
			return storedPaths[startToGoal];
	}
}

void PathFinding::calculatePath(array<float, 3> a, array<float, 3> b)
{	
	// Turn coords to node indexes
	node n[2];
	for (unsigned int i=0; i<nodes.size(); i++)
	{
		if (a == nodes[i].pos)
			n[0] = nodes[i];
		if (b == nodes[i].pos)
			n[1] = nodes[i];
	}
	vector<node> visitedNodes;
	// Calculates all viable paths
	nextNode(a, visitedNodes, );

	int shortestIndex = 0;
	float shortestPath;
	for (unsigned int i=0; i<paths.size(); i++)
	{
		float pathLength;
		for (unsigned int j=1; j<paths[i].size(); j++)
		{
			array<float, 3> a = nodes[paths[i][j-1]].pos;
			array<float, 3> b = nodes[paths[i][j]].pos;
			pathLength += sqrt(pow(a[0]-b[0], 2) + pow(a[1]-b[1], 2)); // c = sqrt(a^2 + b^2)
		}

		if (i == 0) // First loop
		{
			shortestPath = pathLength;
		}
		else if (pathLength < shortestPath)
		{
			shortestPath = pathLength;
			shortestIndex = i;
		}
	}
	
	vector<array<float, 3>> calculatedPath;
	for (unsigned int i=0; i<paths[shortestIndex].size(); i++)
		calculatedPath.push_back(paths[shortestIndex][i].pos);
	array<array<float,3>, 2> startToGoal;
	storedPaths[startToGoal] = calculatedPath;
}

void PathFinding::nextNode(node n, vector<node> visited, node goalNode)
{
	for (unsigned int i=0; i<n.available.size(); i++)
	{
		if (n.pos == goalNode.pos) // Reached goal
		{
			visited.push_back(n);
			paths.push_back(visited); // Add this path to viable paths
		}
		for (unsigned int j=0; j<visited.size(); j++)
		{
			if (nodes[n.available[i]].pos != visited[j].pos)
			{
				visited.push_back(n);
				nextNode(nodes[n.available[i]], visited, goalNode);
			}
		}
	}
}