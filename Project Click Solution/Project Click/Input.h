#pragma once

#include <array>

using std::array;

class Input
{
public:
	Input(void);
	~Input(void);

	static void keyBoard(unsigned char key, int x, int y);
	static void specialKeyBoard(int key, int x, int y);
	static void mouseClick(int key, int state, int x, int y);

	static bool				hasClicked;
	static array<float,2>	clickPos;

	static bool				hasPressedSpecial;
	static bool				hasPressed;
	static unsigned char	pressedKey;
	static int				pressedSpecialKey;
};