#include "Game.h"

//Constructor & Destructor:

Game::Game(void){}
Game::~Game(void){}

//////////////////////////////////////////////////////////////////////////////////////////

//Methods:

//Used to initiate the object:
void Game::init() 
{
	setBlocks(0);
	setScore(0);
}

//Used to increse/decrease score:
void Game::editScore(int delta)
{
	score += delta;
}

//Used to increse/decrease number of blocks:
void Game::editBlocks(int delta) 
{
	blocks += delta;
}

//Shows the player information:
void Game::showInfo()
{
	system("CLS");
	std::cout << "Score: " << score << std::endl;
	if (empty())
		std::cout << "Press \"SPACE\" to restart!" << std::endl;
}

//Returns true if there are no blocks left:
bool Game::empty() 
{
	if (blocks <= 0)
		return true;
	else return false;
}

//////////////////////////////////////////////////////////////////////////////////////////

//Setters:

void Game::setScore(int newScore) {score = newScore;}
void Game::setBlocks(int newBlocks) {blocks = newBlocks;}

//////////////////////////////////////////////////////////////////////////////////////////

//Getters

int Game::getScore() {return score;}
int Game::getBlocks() {return blocks;}

//////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////// End of class