#include "PathFinding.h"
#include <iostream>

PathFinding::PathFinding() {}
PathFinding::~PathFinding() {}
/*
void PathFinding::addHouse(House *house)
{
	building.push_back(house);
	crsLns = false;
}

void PathFinding::update()
{
	nodes.clear();
	crsBatchNs.clear();
	paths.clear();
	storedPaths.clear();
	vector<array<float, 3>> pathNodes;
	for (unsigned int i=0; i<building.size(); i++)
	{
		for (unsigned int j=0; j<building[i]->getHouses().size(); j++)
			building[i]->getHouses()[j]->getNodes(pathNodes);
	}

	for (unsigned int i=0; i<pathNodes.size(); i++)
	{
		//vector<int> aIdx; // Available node's indexes
		//findAvailable(pathNodes, i, aIdx);
		node n;
		n.pos = pathNodes[i];
		findAvailable(pathNodes, i, n.available);
		//n.available = aIdx;
		nodes.push_back(n);
	}

	// Remove non-available paths
	for (unsigned int i=1; i<crsBatchNs.size(); i+=2)
	{
		for (unsigned int j=0; j<nodes.size(); j++)
		{
			for (unsigned int k=0; k<nodes[j].available.size(); k++)
			{
				if (crsBatchNs[i-1] == nodes[j].pos && crsBatchNs[i] == nodes[nodes[j].available[k]].pos)
					nodes[j].available.erase(nodes[j].available.begin()+k);
				else if (crsBatchNs[i] == nodes[j].pos && crsBatchNs[i-1] == nodes[nodes[j].available[k]].pos)
					nodes[j].available.erase(nodes[j].available.begin()+k);
			}
		}
	}


	/*vector<array<float,3>> doorNodes;
	// Calculate path between houses
	for (unsigned int i=0; i<building.size(); i++)
	{
		for (unsigned int j=0; j<building[i]->getHouses().size(); j++)
			doorNodes.push_back(building[i]->getHouses()[j]->getDoorNode());
	}

	
	// Calculate paths between all houses
	for (unsigned int i=0; i<doorNodes.size(); i++)
	{
		for (unsigned int ii=doorNodes.size()-1; ii>i; ii--)
			calculatePath(doorNodes[i], doorNodes[ii]);
	}
}

float PathFinding::cF(float a, float b, bool largest) // Compare floats, returns largest if _largest is true
{
	if (largest && a >= b)
		return a;
	else if (!largest && a >= b)
		return b;
	else if (largest && a < b)
		return b;
	else if (!largest && a < b)
		return a;
}

void PathFinding::findAvailable(vector<array<float, 3>> nCoords, int index, vector<int> &available)
{
	for (unsigned int i=0; i<nCoords.size(); i++)
	{
		// Prevent divide by zero
		if (nCoords[i][0] != nCoords[index][0])
		{
			bool avb = false;
			// y=kx+m equation to check if the line between the two points crosses a house-wall
			float k, m; // nCoords[index] contains x and y
			k = (nCoords[index][1]-nCoords[i][1])/(nCoords[index][0]-nCoords[i][0]);
			m = nCoords[index][1]-(k*nCoords[index][0]);
			
			// Check if line crosses a wall
			for (unsigned int j=0; j<building.size(); j++)
			{
				for (unsigned int jj=0; jj<building[j]->getHouses().size(); jj++)
				{
					array<float, 4> walls = building[j]->getHouses()[jj]->getWalls();
					// Check if both points are on the same side of the cube
					if (cF(nCoords[index][0], nCoords[i][0], true) < walls[0] || cF(nCoords[index][0], nCoords[i][0], false) > walls[1] || cF(nCoords[index][1], nCoords[i][1], true) < walls[3] || cF(nCoords[index][1], nCoords[i][1], false) > walls[2])
					{
						avb = true;
					}
					else
					{
						// Check if it crosses either wall
						if (!(((k*walls[0])+m <= walls[3] && (k*walls[0])+m >= walls[2]) || // Crosses left wall
							((k*walls[1])+m <= walls[3] && (k*walls[1])+m >= walls[2]) || // Crosses right wall
							((walls[2]-m)/k <= walls[0] && (walls[2]-m)/k <= walls[1]) || // Crosses lower wall
							((walls[3]-m)/k <= walls[0] && (walls[3]-m)/k <= walls[1]))) // Crosses upper wall
						{
							avb = true;
						}
						else
						{
							array<float, 3> pos = nCoords[index];
							crsBatchNs.push_back(pos);
							pos = nCoords[i];
							crsBatchNs.push_back(pos);
						}
					}
				}
			}
			if (avb)
				available.push_back(i);
		}
		else if (index != i)
		{
			bool avb = false;
			for (unsigned int j=0; j<building.size(); j++)
			{
				for (unsigned int jj=0; jj<building[j]->getHouses().size(); jj++)
				{
					array<float, 4> walls = building[j]->getHouses()[jj]->getWalls();
					// Check that atleast 1 point is inside/on the other side of the building
					if (!(cF(nCoords[index][1], nCoords[i][1], false) > walls[3] || cF(nCoords[index][1], nCoords[i][1], true) < walls[2]))
					{
						// Check if this path goes through any wall
						if (!(nCoords[i][0] <= walls[1] && nCoords[i][0] >= walls[0]))
						{
							avb = true;
						}
						else
						{
							array<float, 3> pos = nCoords[index];
							crsBatchNs.push_back(pos);
							pos = nCoords[i];
							crsBatchNs.push_back(pos);
						}
					}
					else
					{
						avb=true;
					}
				}
			}
			if (avb)
				available.push_back(i);
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
	nextNode(n[0], visitedNodes, n[1]);

	int shortestIndex = 0;
	float shortestPath;
	// Loop through all paths
	for (unsigned int i=0; i<paths.size(); i++)
	{
		float pathLength;
		// Loop through all nodes, begin at second node
		for (unsigned int j=1; j<paths[i].size(); j++)
		{
			array<float, 3> pt1 = paths[i][j-1].pos;
			array<float, 3> pt2 = paths[i][j].pos;
			pathLength += sqrt(pow(pt1[0]-pt2[0], 2) + pow(pt1[1]-pt2[1], 2)); // c = sqrt(a^2 + b^2)
			std::cout << "Pathlength: " << pathLength << std::endl; 
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
	paths.clear(); // Clean up for next path-calculation
}

void PathFinding::nextNode(node n, vector<node> visited, node goalNode)
{
	visited.push_back(n);
	// Loop through all available nodes
	for (unsigned int i=0; i<n.available.size(); i++)
	{
		if (n.pos == goalNode.pos) // Reached goal
			paths.push_back(visited); // Add this path to viable paths

		for (unsigned int j=0; j<visited.size(); j++)
		{
			if (nodes[n.available[i]].pos != visited[j].pos)
			{
				nextNode(nodes[n.available[i]], visited, goalNode);
			}
		}
	}
}

void PathFinding::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	if (storedPaths.size() > 0) // Any stored paths
	{
		// Colour of the pathLines
		float vColour[] = {0.2f, 0.8f, 0.2f, 1.0f};
		// Loop through all paths
		typedef map<array<array<float,3>, 2>, vector<array<float, 3>>>::iterator mIter;
		for (mIter iter=storedPaths.begin(); iter!=storedPaths.end(); iter++)
		{
			// Loop through the path
			for (unsigned int i=1; i<iter->second.size(); i++)
			{
				// Generate batch between the two points
				pathBatch.Begin(GL_LINE, 2);
				pathBatch.Vertex3f(iter->second[i-1][0], iter->second[i-1][1], iter->second[i-1][2]);
				pathBatch.Vertex3f(iter->second[i][0], iter->second[i][1], iter->second[i][2]);
				pathBatch.End();
				
				glDisable(GL_DEPTH_TEST);
				mvStack.PushMatrix();
				shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), vColour);
				pathBatch.Draw();
				mvStack.PopMatrix();
				glEnable(GL_DEPTH_TEST);
			}
		}
	}
	glDisable(GL_DEPTH_TEST);
	float aLineColour[] = {0.2f, 0.8f, 0.2f, 1.0f}; // Green
	std::auto_ptr<GLBatch> aLineBatch (new GLBatch);
	aLineBatch->Begin(GL_LINES, pow(nodes.size(), 3));
	// Generate batch between the two points
	for (unsigned int i=0; i<nodes.size(); i++)
	{
		for (unsigned int j=0; j<nodes[i].available.size(); j++)
		{
			aLineBatch->Vertex3f(nodes[i].pos[0], nodes[i].pos[1], nodes[i].pos[2]);
			aLineBatch->Vertex3f(nodes[nodes[i].available[j]].pos[0], nodes[nodes[i].available[j]].pos[1], nodes[nodes[i].available[j]].pos[2]);
			
		}
	}
	aLineBatch->End();
	//glDisable(GL_DEPTH_TEST);
	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), aLineColour);
	aLineBatch->Draw();
	mvStack.PopMatrix();
	//glEnable(GL_DEPTH_TEST);
	aLineBatch->Reset();

	// Just for debugging below
	bool drawWalls = true;
	if (drawWalls)
	{
		for (unsigned int i=0; i<building.size(); i++)
		{
			for (unsigned int j=0; j<building[i]->getHouses().size(); j++)
			{
				array<float, 4> walls = building[i]->getHouses()[j]->getWalls();
				float wallLineColour[] = {0.8f, 0.2f, 0.2f, 1.0f}; // Red
				GLBatch wallBatch;
				wallBatch.Begin(GL_LINE_LOOP, 4);
				wallBatch.Vertex3f(walls[0], walls[3], 0.0f);
				wallBatch.Vertex3f(walls[1], walls[3], 0.0f);
				wallBatch.Vertex3f(walls[1], walls[2], 0.0f);
				wallBatch.Vertex3f(walls[0], walls[2], 0.0f);
				wallBatch.End();
				
				//glDisable(GL_DEPTH_TEST);
				mvStack.PushMatrix();
				shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), wallLineColour);
				wallBatch.Draw();
				mvStack.PopMatrix();
				//glEnable(GL_DEPTH_TEST);
				wallBatch.Reset();
			}
		}
	}

	if (crsLns)
	{
		for (unsigned int i=1; i<crsBatchNs.size(); i+=2)
		{
			float cLineColour[] = {0.8f, 0.8f, 0.2f, 1.0f}; // Yellow
			GLBatch cLineBatch;
			cLineBatch.Begin(GL_LINES, 2);
			cLineBatch.Vertex3f(crsBatchNs[i-1][0], crsBatchNs[i-1][1], crsBatchNs[i-1][2]);
			cLineBatch.Vertex3f(crsBatchNs[i][0], crsBatchNs[i][1], crsBatchNs[i][2]);
			cLineBatch.End();
			
			//glDisable(GL_DEPTH_TEST);
			mvStack.PushMatrix();
			shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), cLineColour);
			cLineBatch.Draw();
			mvStack.PopMatrix();
			//glEnable(GL_DEPTH_TEST);
			cLineBatch.Reset();
		}
	}
	glEnable(GL_DEPTH_TEST);
}
*/
//void PathFinder::sizes()
//{
//	for (int i)
//}