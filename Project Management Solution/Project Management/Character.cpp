#include "Character.h"

Character::Character(void)
{
}

Character::~Character(void)
{
}

void Character::init(float charSize, float mvSpeed, M3DVector3f &spawnPos)
{
	size = charSize;
	mvmSpeed = mvSpeed;
	cFrame.SetOrigin(spawnPos);
	pathStepSize = 0.01f;
}

void Character::moveTo(M3DVector3f mvPos)
{
	calculatePathLine(mvPos);
}

void Character::calculatePathLine(M3DVector3f mvPos)
{
	vector< array<float, 3> > pathNodes; 
	array<float, 3> sPos, ePos; // Starting/actual and ending position
	sPos[0] = cFrame.GetOriginX();
	sPos[1] = cFrame.GetOriginY();
	sPos[2] = cFrame.GetOriginZ();
	pathNodes.push_back(sPos);
	ePos[0] = mvPos[0];
	ePos[1] = mvPos[1];
	ePos[2] = mvPos[2];
	float xDiff, yDiff;
	
	// Diraction towards endPos
	float alpha = abs(atan2f(mvPos[1]-sPos[1], mvPos[0]-sPos[0]));
	// Next path-pos
	while ((pathNodes[pathNodes.size()-1][0] != ePos[0]) && (pathNodes[pathNodes.size()-1][1] != ePos[1]))
	{
		if (ePos[0] >= sPos[0] && ePos[1] >= sPos[1])
		{
			xDiff = pathStepSize*cos(alpha);
			yDiff = pathStepSize*sin(alpha);
		}
		else if (ePos[0] <= sPos[0] && ePos[1] >= sPos[1])
		{
			xDiff = -pathStepSize*cos(alpha);
			yDiff = pathStepSize*sin(alpha);
		}
		else if (ePos[0] >= sPos[0] && ePos[1] <= sPos[1])
		{
			xDiff = pathStepSize*cos(alpha);
			yDiff = -pathStepSize*sin(alpha);
		}
		else if (ePos[0] <= sPos[0] && ePos[1] <= sPos[1])
		{
			xDiff = -pathStepSize*cos(alpha);
			yDiff = -pathStepSize*sin(alpha);
		}
		// Update position
		sPos[0] += xDiff;
		sPos[1] += yDiff;
		pathNodes.push_back(sPos);
	}
	generatePath(pathNodes, ePos);
}

void Character::generatePath(vector< array<float, 3> > pathNodes, array<float, 3> goalPos) // May need optimisation
{
	// Check if any objects are in the way
	for (unsigned int i=0; i < pathNodes.size(); i++)
	{
		for (unsigned int j=0; j < objects.size(); j++)
		{
			if ((pathNodes[i][0] >= objects[j]->pos[0]-objects[i]->rad) && // Inside an object
				(pathNodes[i][0] <= objects[j]->pos[0]+objects[i]->rad) &&
				(pathNodes[i][1] >= objects[j]->pos[1]-objects[i]->rad) &&
				(pathNodes[i][1] <= objects[j]->pos[1]+objects[i]->rad));
			{
				//
			}
		}
	}
}