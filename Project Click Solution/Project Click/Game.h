#pragma once
#include <iostream>

class Game //Class for handling gameplay related stuff, like score for example
{
	public:
		Game(void);
		~Game(void);

		void init();					//Initiates the object
		void editScore(int delta);		//Value to be added (negative or positive)
		void editBlocks(int delta);		//Value to be added (negative or positive)
		void showInfo();

		void setScore(int newScore);	//Set to zero to "reset"
		void setBlocks(int nBlocks);	//Sets number of blocks currently in-game

		int getScore();					//Returns the score
		int getBlocks();				//Returns the number of blocks
		bool empty();

	private:
		int score;						//The current player score
		int blocks;						//The current number of blocks
};

