#pragma once

class Globals
{
	public:
		Globals();
		~Globals();

		enum Gamestate
		{
			STATE_ALPHA,
			STATE_SURVIVAL,
			STATE_MENU
		};

		static Gamestate state;
		static int nBlocks;
		static int lives;
};