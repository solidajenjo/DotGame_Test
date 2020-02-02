#include "game.h"
#include <resources.h>
#include <iostream>
#include <globals.h>
#include <modules.h>
#include <behaviours.h>
#include <dot.h>
#include <time.h>
#include <cassert>

Game::Game(ResourceManager& rm) : RM(rm)
{
	//Init game
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0 || TTF_Init() < 0)
	{
		std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}
	auto renderModule = new RenderModule(RM);
	modules.emplace("renderer", renderModule);
	RM.SetRenderer(renderModule->GetRenderer());

	std::vector<ResourceItem>resources
	{
		{eResourceType::rTexture, "greenDot", "resources/textures/ClickableGreen.png"},
		{eResourceType::rTexture, "redDot", "resources/textures/ClickableRed.png"},
		{eResourceType::rTexture, "tileBG", "resources/textures/TileBackground.png"},
		{eResourceType::rTexture, "tileMO", "resources/textures/TileMouseOver.png"}
	};

	RM.LoadResources(resources);

	modules.emplace("gameBoard", new GameBoardModule(RM, HORIZONTAL_PADDING, VERTICAL_PADDING));

	modules.emplace("input", new InputModule());

	modules.emplace("gameControl", new GameControlModule());
	
	modules.emplace("gameLogic", new GameLogicModule());

	srand(time(NULL));
}


bool Game::Update()
{

	for (auto m : modules)
	{
		m.second->Update(*this);
	}

	for (auto m : modules)
	{
		m.second->PostUpdate();
	}

	auto logic = (GameLogicModule*)modules.at("gameLogic");
	assert(logic);

	if (!logic->running)
		logic->Reset();

	return !(((InputModule*)modules.at("input"))->quit || quit);
}

void Game::Clear()
{
	for (auto m : modules)
	{
		delete m.second;
	}
	modules.clear();
}






