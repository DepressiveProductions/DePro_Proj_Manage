#pragma once

#include "MenuRend.h"
#include "MenuInp.h"

class Menu : private MenuRend, private MenuInp
{
public:
	Menu(void);
	~Menu(void);
};

