#include "Path.h"

Path::Path(void) {}

Path::~Path(void) {}

void Path::addBuildingType(House *house)
{
	buildingTypes.push_back(house);
}

void Path::update()
{
	copyNodes();
	setAvailable();
}

// Adds all nodes from houses to the nodes-vector
void Path::copyNodes()
{
	nodes.clear(); // Erase the earlier nodes
	for (unsigned int i=0; i<buildingTypes.size(); i++)
	{
		for (unsigned int j=0; j<buildingTypes[i]->getHouses().size(); j++)
		{
			node n; // Temporary node for the position
			n.pos = buildingTypes[i]->getHouses()[j]->getNodes()[0]; // Add the first house-node
			nodes.push_back(n);
			n.pos = buildingTypes[i]->getHouses()[j]->getNodes()[1]; // Add the second house-node
			nodes.push_back(n);
			n.pos = buildingTypes[i]->getHouses()[j]->getNodes()[2]; // Add the third house-node
			nodes.push_back(n);
			n.pos = buildingTypes[i]->getHouses()[j]->getNodes()[3]; // Add the fourth house-node
			nodes.push_back(n);
			n.pos = buildingTypes[i]->getHouses()[j]->getNodes()[4]; // Add the door-node
			nodes.push_back(n);
		}
	}
}

void Path::setAvailable()
{
	for (unsigned int n=0; n<nodes.size(); n++)
	{
		for (unsigned int nn=n+1; nn<nodes.size(); nn++)
		{
			bool valid = true;
			for (unsigned int bt=0; bt<buildingTypes.size(); bt++)
			{
				for (unsigned int b=0; b<buildingTypes[bt]->getHouses().size(); b++)
				{
					if (pValid(nodes[n].pos, nodes[nn].pos, buildingTypes[bt]->getHouses()[b]->getWalls()))
					{
						valid = true;
						continue;
					}

					if (pInvalid(nodes[n].pos, nodes[nn].pos, buildingTypes[bt]->getHouses()[b]->getWalls()))
					{
						valid = false;
						break;
					}

					if (!lineCheck(nodes[n].pos, nodes[nn].pos, buildingTypes[bt]->getHouses()[b]->getWalls()))
					{
						valid = false;
						break;
					}
				}
				if (!valid)
					break;
			}
			if (valid)
			{
				nodes[nn].available.push_back(&nodes[n]);
				nodes[n].available.push_back(&nodes[nn]);
			}
		}
	}
}

bool Path::pValid(array<float,3> p1, array<float,3> p2, array<float,4> walls)
{
	if ((p1[0] < walls[0] && p2[0] < walls[0]) || (p1[0] > walls[1] && p2[0] > walls[1]))
	{
		if ((p1[1] < walls[2] && p2[1] < walls[2]) || (p1[1] > walls[3] && p2[1] > walls[3]))
			return true;
	}
	return false;
}

bool Path::pInvalid(array<float,3> p1, array<float,3> p2, array<float,4> walls)
{
	// Check x
	if (p1[0] >= walls[0] && p1[0] <= walls[1] && p2[0] >= walls[0] && p2[0] <= walls[1])
	{
		if (!((p1[1] > walls[3] && p2[1] > walls[3]) || (p1[1] < walls[2] && p2[1] < walls[2])))
			return true;
	}
	// Check y
	if (p1[1] >= walls[2] && p1[1] <= walls[3] && p2[1] >= walls[2] && p2[1] <= walls[3])
	{
		if (!((p1[0] > walls[1] && p2[0] > walls[1]) || (p1[0] < walls[0] && p2[0] < walls[0])))
			return true;
	}
	return false;
}

bool Path::lineCheck(array<float,3> p1, array<float,3> p2, array<float,4> walls) // Returns true if valid
{
	array<float,2> vDir = {p1[0]-p2[0], p1[1]-p2[1]}; // (v1, v2)

	float var; //Comparison var for x/y

	//Left wall:
	var = applyLine(vDir[0], vDir[1], walls[0], p1[0], p1[1]);
	if (var >= walls[2] && var <= walls[3])
		return false;

	//Right wall:
	var = applyLine(vDir[0], vDir[1], walls[1], p1[0], p1[1]);
	if (var >= walls[2] && var <= walls[3])
		return false;

	//Bottom wall:
	var = applyLine(vDir[1], vDir[0], walls[2], p1[1], p1[0]);
	if (var >= walls[0] && var <= walls[1])
		return false;

	//Top wall:
	var = applyLine(vDir[1], vDir[0], walls[3], p1[1], p1[0]);
	if (var >= walls[0] && var <= walls[1])
		return false;
	
	return true;
}

float Path::applyLine(float v1, float v2, float wall, float cVar1, float cVar2) // Returns x/y for comparison
{
	return cVar2 + ((wall -cVar1)/v1)*v2; // t = (wall -cVar1)/v1
}

void Path::draw(GLShaderManager &shaderManager, GLGeometryTransform &tPipeline, GLMatrixStack &mvStack)
{
	glDisable(GL_DEPTH_TEST);
	float aLineColour[] = {0.2f, 0.8f, 0.2f, 1.0f}; // Green
	GLBatch aLineBatch;
	
	int alloc = 0;
	for (unsigned int i=0; i<nodes.size(); i++)
		alloc += nodes[i].available.size()*2;
	aLineBatch.Begin(GL_LINES, alloc);
	// Generate batch between the two points
	for (unsigned int i=0; i<nodes.size(); i++)
	{
		for (unsigned int j=0; j<nodes[i].available.size(); j++)
		{
			aLineBatch.Vertex3f(nodes[i].pos[0], nodes[i].pos[1], nodes[i].pos[2]);
			aLineBatch.Vertex3f(nodes[i].available[j]->pos[0], nodes[i].available[j]->pos[1], nodes[i].available[j]->pos[2]);
			
		}
	}
	aLineBatch.End();
	mvStack.PushMatrix();
	shaderManager.UseStockShader(GLT_SHADER_FLAT, tPipeline.GetModelViewProjectionMatrix(), aLineColour);
	aLineBatch.Draw();
	mvStack.PopMatrix();
	aLineBatch.Reset();

	// Just for debugging below
	bool drawWalls = true;
	if (drawWalls)
	{
		for (unsigned int i=0; i<buildingTypes.size(); i++)
		{
			for (unsigned int j=0; j<buildingTypes[i]->getHouses().size(); j++)
			{
				array<float, 4> walls = buildingTypes[i]->getHouses()[j]->getWalls();
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
	glEnable(GL_DEPTH_TEST);
}