#pragma once
class Input
{
public:
	Input(void);
	~Input(void);

	void keyBoard();										//Handles keyboard input
	void mouseClick(int key, int state, int x, int y);		//Handles mouse clicking


};