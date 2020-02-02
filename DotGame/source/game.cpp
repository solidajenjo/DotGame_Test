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
		{eResourceType::rTexture, "tileMO", "resources/textures/TileMouseOver.png"},
		{eResourceType::rFont, "font", "resources/fonts/font.ttf|28"}
	};

	RM.LoadResources(resources);

	modules.emplace("gameBoard", new GameBoardModule(RM, HORIZONTAL_PADDING, VERTICAL_PADDING));

	modules.emplace("input", new InputModule());

	modules.emplace("gameControl", new GameControlModule());
	
	modules.emplace("gameLogic", new GameLogicModule());

	srand(time(NULL));

	SetTimer();
}


bool Game::Update()
{
	auto logic = (GameLogicModule*)modules.at("gameLogic");
	assert(logic);

	switch (state) {
		case States::INTRO:
		{
			GetModule<RenderModule>("renderer")->RenderText((*RM.GetResource<Font>("font")).GetTTFFont(), "Ruben Ajenjo presents: The Dot Game",
				{ 255, 255, 255, 255 }, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 40, 15);

			timer -= logic->deltaTime;
			if (timer < 0.f)
			{
				state = States::PLAYING;
				logic->Reset();
			}
			break;
		}
		case States::PLAYING:
		{
			for (auto m : modules)
			{
				m.second->Update(*this);
			}

			if (gameOver)
			{
				state = States::GAME_OVER;
				SetTimer();
			}
			break;
		}
		case States::GAME_OVER:
		{
			GetModule<RenderModule>("renderer")->RenderText((*RM.GetResource<Font>("font")).GetTTFFont(), "Game over!",
				{ 255, 255, 255, 255 }, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 40, 35);
			std::string scoreMessage = "Your final score: " + std::to_string(logic->score);
			GetModule<RenderModule>("renderer")->RenderText((*RM.GetResource<Font>("font")).GetTTFFont(), scoreMessage,
				{ 255, 255, 255, 255 }, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 30);

			timer -= logic->deltaTime;
			if (timer < 0.f)
			{
				state = States::INTRO;
				gameOver = false;
				SetTimer();
			}
			break;
		}
	}
	for (auto m : modules)
	{
		m.second->PostUpdate();
	}
	return !(((InputModule*)modules.at("input"))->quit);
}

void Game::Clear()
{
	auto logic = (GameLogicModule*)modules.at("gameLogic");
	assert(logic);
	logic->Reset();
	RM.Clear();
	for (auto m : modules)
	{
		delete m.second;
	}	
	modules.clear();
}

void Game::SetTimer()
{
	timer = SPLASH_SCREEN_DURATION;
}






