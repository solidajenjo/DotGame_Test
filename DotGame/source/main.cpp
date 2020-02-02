#include <game.h>
#include <resources.h>
#undef main

int main(int argc, char* argv[])
{
	ResourceManager RM;
	Game game(RM);
	while (game.Update());
	game.Clear();
	return 0;
}