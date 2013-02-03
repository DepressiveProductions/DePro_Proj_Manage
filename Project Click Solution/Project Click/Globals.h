#pragma once

class Globals
{
	public:
		Globals();
		~Globals();

		enum Gamestate
		{
			STATE_PLAY,
			STATE_MENU
		};

		static Gamestate state;
		static int nBlocks;
};