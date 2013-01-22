#pragma once

#include "PlayInp.h"
#include "PlayRend.h"

class Play : private PlayInp, private PlayRend
{
public:
	Play(void);
	~Play(void);
};

