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
struct SDL_Color; 
typedef struct _TTF_Font TTF_Font;


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

	void Update(const Game& game) override {}
	void PostUpdate() override;

	void RenderTexture(Texture& texture, int x, int y) const;
	void RenderText(TTF_Font* font, const std::string& text, const SDL_Color& color, uint16_t x, uint16_t y, uint16_t fontSize) const;
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
	
	int cellW, cellH;

private:
	uint16_t startX, startY, leftMargin, rightMargin, topMargin, bottomMargin;
	
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
	void PostUpdate() override;

	void Stop();
	void Reset();
	void GetDotDestination(const Game& game, uint16_t& x, uint16_t& y) const;
	inline DPtr GetLastDot() const { return *(--dots.end()); };

	float deltaTime;

	uint8_t cellX, cellY;
	Dot* activeDot = nullptr;
	bool running = false;
	uint8_t score;

private:

	std::vector<DPtr> dots;
	uint32_t lastTime = 0;
	float timer;

};
