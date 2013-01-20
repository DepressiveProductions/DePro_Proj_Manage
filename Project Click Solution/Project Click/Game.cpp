#include "Game.h"

//Constructor & Destructor:
Game::Game(void){}
Game::~Game(void){}

//Methods:

void Game::init() 
{
	setBlocks(0);
	setScore(0);
	blocksLeft = false;
}

void Game::editScore(int delta)
{
	score += delta;
}

void Game::editBlocks(int delta) 
{
	blocks += delta;
	if (blocks <= 0) blocksLeft = false;
	else blocksLeft = true;
}

void Game::showInfo()
{
	system("CLS");
	std::cout << "Score: " << score << std::endl;
	if (!blocksLeft)
		std::cout << "Press \"SPACE\" to restart!" << std::endl;
}

//Setters:

void Game::setScore(int newScore) {score = newScore;}
void Game::setBlocks(int nBlocks) {blocks = nBlocks;}

//Getters

int Game::getScore() {return score;}
int Game::getBlocks() {return blocks;}
bool Game::notEmpty() {return blocksLeft;}