#include <modules.h>
#include <SDL.h>
#include <globals.h>
#include <SDL_ttf.h>
#include <resources.h>
#include <game.h>
#include <iostream>
#include <cassert>
#include <math.h>
#include <behaviours.h>
#include <dot.h>

RenderModule::RenderModule(ResourceManager& rm)
{
	window = SDL_CreateWindow("Dot Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

RenderModule::~RenderModule()
{
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}


void RenderModule::PostUpdate()
{
	SDL_RenderPresent(renderer);
}

void RenderModule::RenderTexture(Texture& texture, int x, int y) const
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = texture.w;
	rect.h = texture.h;

	SDL_RenderCopyEx(renderer, texture.GetTexture() , nullptr, &rect, 0, nullptr, SDL_FLIP_NONE);
}

GameBoardModule::GameBoardModule(ResourceManager& rm, uint16_t startX, uint16_t startY) : startX(startX), startY(startY)
{
	auto cell = rm.GetResource<Texture>("tileBG");
	assert(cell);

	leftMargin = HORIZONTAL_PADDING;
	rightMargin = HORIZONTAL_PADDING + HORIZONTAL_CELLS * cell->w;
	topMargin = VERTICAL_PADDING;
	bottomMargin = VERTICAL_PADDING + VERTICAL_CELLS * cell->h;

	cellW = cell->w;
	cellH = cell->h;
}

void GameBoardModule::Update(const Game& game)
{
	auto cell = game.RM.GetResource<Texture>("tileBG");
	auto renderModule = game.GetModule<RenderModule>("renderer");
	assert(cell && renderModule);

	for (size_t x = 0; x < HORIZONTAL_CELLS; ++x)
	{
		int xPos = HORIZONTAL_PADDING + int(x) * cell->w;
		for (size_t y = 0; y < VERTICAL_CELLS; ++y)
		{
			int yPos = VERTICAL_PADDING + int(y) * cell->h;
			renderModule->RenderTexture(*cell, xPos, yPos);
		}
	}
}

bool GameBoardModule::IsOverCell(int mouseX, int mouseY, uint8_t& cellX, uint8_t& cellY) const
{

	if (mouseX > leftMargin&& mouseX < rightMargin && mouseY > topMargin&& mouseY < bottomMargin)
	{
		mouseX -= HORIZONTAL_PADDING;
		cellX = mouseX / cellW;

		mouseY -= VERTICAL_PADDING;
		cellY = mouseY / cellH;

		return true;
	}
	return false;
}

void GameBoardModule::GetCellScreenPos(uint8_t cellX, uint8_t cellY, int& x, int& y) const
{
	x = HORIZONTAL_PADDING + cellX * cellW;
	y = VERTICAL_PADDING + cellY * cellH;
}

void GameBoardModule::GetRandomCell(uint8_t& cellX, uint8_t& cellY, int& x, int& y)
{
	cellX = rand() % HORIZONTAL_CELLS;
	cellY = rand() % VERTICAL_CELLS;
	GetCellScreenPos(cellX, cellY, x, y);
}

void InputModule::Update(const Game& game)
{
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			quit = event.key.keysym.sym == SDLK_ESCAPE;
			break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				leftClick = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				rightClick = true;
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				leftClick = false;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				rightClick = false;
			break;
		}
		default:
			break;
		}
	}
}

bool InputModule::LeftClick()
{
	bool state = leftClick;
	leftClick = false;
	return state;
}

bool InputModule::RightClick()
{
	bool state = rightClick;
	rightClick = false;
	return state;
}

void GameControlModule::Update(const Game& game)
{
	auto board = game.GetModule<GameBoardModule>("gameBoard");
	auto input = game.GetModule<InputModule>("input");
	auto render = game.GetModule<RenderModule>("renderer");
	auto logic = game.GetModule<GameLogicModule>("gameLogic");

	auto cell = game.RM.GetResource<Texture>("tileBG");
	auto cellMO = game.RM.GetResource<Texture>("tileMO");
	assert(board && input && render && logic && cell && cellMO);

	uint8_t xCell, yCell;
	if (board->IsOverCell(input->mouseX, input->mouseY, xCell, yCell))
	{
		int x, y;
		board->GetCellScreenPos(xCell, yCell, x, y);
		render->RenderTexture(*cellMO, x, y);
		bool leftClick = input->LeftClick();
		bool rightClick = input->RightClick();
		if (xCell == logic->cellX && yCell == logic->cellY)
		{
			if (leftClick)
			{
				if (logic->activeDot && logic->activeDot->isLeft)
				{
					std::cout << "HIT LEFT" << std::endl;
					logic->activeDot = nullptr;
				}
				else
				{
					std::cout << "LEFT MISS " << std::endl;
					game.quit = true;
				}
			}
			else if (rightClick)
			{	

				if (logic->activeDot && !logic->activeDot->isLeft)
				{
					std::cout << "HIT RIGHT" << std::endl;
					logic->activeDot = nullptr;
				}
				else
				{
					std::cout << "RIGHT MISS " << std::endl;
					game.quit = true;
				}
			}
		}
		else if (leftClick || rightClick) {
			std::cout << "FAIL" << std::endl;
			game.quit = true;
		}
		
	}
	
}

GameLogicModule::~GameLogicModule()
{
	Reset();
}

void GameLogicModule::Update(const Game& game)
{
	uint32_t time = SDL_GetPerformanceCounter();
	deltaTime = (time - lastTime) / (float)SDL_GetPerformanceFrequency();
	lastTime = time;
	
	timer -= deltaTime;

	if (timer <= 0.0f && running)
	{
		if (activeDot)
		{
			std::cout << "TOO SLOW" << std::endl;
			game.quit = true;
		}
		
		float colorProb = RAND_FLOAT;
		bool isLeft;

		if (colorProb < 0.5f)
			isLeft = true;
		else
			isLeft = false;

		auto d = std::make_shared<Dot>(isLeft);
		activeDot = d.get();
		auto board = game.GetModule<GameBoardModule>("gameBoard");
		assert(board);

		int x, y;

		board->GetRandomCell(cellX, cellY, x, y);

		if (isLeft)
			d->AddBehaviour(new SpriteBehaviour(game.RM.GetResource<Texture>("greenDot"), x, y));
		else
			d->AddBehaviour(new SpriteBehaviour(game.RM.GetResource<Texture>("redDot"), x, y));

		dots.push_back(d);
		timer = DOT_DURATION;
	}

	for (auto d : dots)
	{
		d->Update(game);
	}
}

void GameLogicModule::Reset()
{
	timer = DOT_DURATION;
	dots.clear();
	running = true;
	activeDot = nullptr;
}
