#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

class ResourceManager;
class Game;
class Texture;
class Dot;

struct SDL_Window;
struct SDL_Renderer;

typedef std::shared_ptr<Dot> DPtr;

class Module
{	
public:

	virtual ~Module() {};
	virtual void Update(const Game& game) = 0;
	virtual void PostUpdate() {}
	
};

class RenderModule : public Module
{
public:
	RenderModule(ResourceManager& rm);
	~RenderModule();

	void Update(const Game& game) {}
	void PostUpdate() override;

	void RenderTexture(Texture& texture, int x, int y) const;
	SDL_Renderer* GetRenderer() const { return renderer; }

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};


class GameBoardModule : public Module
{
public:
	GameBoardModule(ResourceManager& rm, uint16_t startX, uint16_t startY);
	void Update(const Game& game) override;

	bool IsOverCell(int mouseX, int mouseY, uint8_t& cellX, uint8_t& cellY) const;
	void GetCellScreenPos(uint8_t cellX, uint8_t cellY, int& x, int& y) const;
	void GetRandomCell(uint8_t& cellX, uint8_t& cellY, int& x, int& y);

private:
	uint16_t startX, startY, leftMargin, rightMargin, topMargin, bottomMargin;
	int cellW, cellH;
};

class InputModule : public Module
{
public:
	void Update(const Game& game) override;

	bool LeftClick();
	bool RightClick();

	bool quit = false;

	int mouseX, mouseY;

private:
	bool leftClick = false;
	bool rightClick = false;
};

class GameControlModule : public Module
{
public:
	void Update(const Game& game) override;
};

class GameLogicModule : public Module
{
public:

	~GameLogicModule();

	void Update(const Game& game) override;

	void Reset();

	float deltaTime;

	uint8_t cellX, cellY;
	Dot* activeDot = nullptr;
	bool running = false;

private:

	std::vector<DPtr> dots;
	uint32_t lastTime = 0;
	float timer;

};
