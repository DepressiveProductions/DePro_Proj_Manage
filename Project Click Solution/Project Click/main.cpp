#include <GLTools.h>

void setupRC();											//One-time setup function (RC = Rendering Context)
void changeSize(int w, int h);							//Runs everytime the window 'changes size', for example when the window is created
void renderScene();										//Basic glutfunc for rendering stuff. Runs every frame
void handleInput();										//Handles input ...
void clickFunc();