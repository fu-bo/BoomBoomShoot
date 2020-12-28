#include "Game.hpp"
#include "MyDraw.hpp"
#include <time.h>

int main(void){

    MyDraw::Srand((int)time(NULL));

	Game game;

	game.Initialize();
	game.OpenWindow();
	while(!game.Terminate()){
		game.Run();
	}

	return 0;
}