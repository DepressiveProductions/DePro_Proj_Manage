#pragma once
#include <GLTools.h>
#include <GLMatrixStack.h>
#include <GLShaderManager.h>
#include <GLGeometryTransform.h>
#include <GLFrustum.h>
#include <GLFrame.h>
#include <GLBatch.h>
#include <vector>
#include <array>
using std::vector;
using std::array;

class Character
{
public:
	Character(void);
	~Character(void);
	void init(float charSize, float mvSpeed, M3DVector3f &spawnPos);
	void moveTo(M3DVector3f mvPos);

private:
	GLFrame cFrame;
	GLBatch cBatch;
	float size, mvmSpeed;
	struct object {M3DVector3f pos; float rad;};
	vector<object *> objects;

	vector< array<float, 3> > path;
	void calculatePathLine(M3DVector3f mvPos);
	void generatePath(vector< array<float, 3> > pathNodes, array<float, 3> goalPos);
	float pathStepSize;
};

