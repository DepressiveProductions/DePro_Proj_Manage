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
			STATE_MENU,
			STATE_OPTIONS
		};

		static Gamestate state;
		static int nBlocks;
		static int lives;
		static float speed;
		static int score;
};