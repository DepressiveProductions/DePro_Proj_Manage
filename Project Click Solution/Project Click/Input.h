#pragma once

#include <array>
#include <GLTools.h>

using std::array;

class Input
{
public:
	Input(void);
	~Input(void);

	static void				keyBoard(unsigned char key, int x, int y);
	static void				specialKeyBoard(int key, int x, int y);
	static void				mouseClick(int key, int state, int x, int y);
	
	static array<float,3>	checkClicked(int x, int y, M3DMatrix44f mCamera, M3DMatrix44f mProj);

	static bool				hasClicked;
	static array<int,2>		clickPos;

	static bool				hasPressedSpecial;
	static bool				hasPressed;
	static unsigned char	pressedKey;
	static int				pressedSpecialKey;
};