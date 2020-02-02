#pragma once
#include <stdint.h>
#include <memory>

class Texture;
class Game;
class Dot;

typedef std::shared_ptr<Dot> DPtr;

class Behaviour
{
public :

	enum class BehaviourTypes
	{
		NONE,
		SPRITE = 1,
		TRANSLATE = 1 << 1
	};

	BehaviourTypes type = BehaviourTypes::NONE;

	Behaviour(DPtr owner) : owner(owner) {};

	virtual ~Behaviour() {}
	virtual void Update(const Game& game) = 0;

	DPtr owner = nullptr;	
	bool done = false;
};

class SpriteBehaviour : public Behaviour
{
public:

	SpriteBehaviour(DPtr owner, std::shared_ptr<Texture> t, uint16_t x, uint16_t y) : Behaviour(owner), texture(t), x(x), y(y) { type = BehaviourTypes::SPRITE; };

	void Update(const Game& game) override;		
	void SetPosition(uint16_t nx, uint16_t ny) { x = nx; y = ny; };

private:
	std::shared_ptr<Texture> texture;
	uint16_t x, y;
};

class TranslateBehaviour : public Behaviour
{
public:

	TranslateBehaviour(DPtr owner, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, float time) : Behaviour(owner), x0(x0), y0(y0), x1(x1), y1(y1), time(time) { type = BehaviourTypes::TRANSLATE; }

	void Update(const Game& game) override;

private:
	uint16_t x0, y0, x1, y1;
	float time;
	float timePassed = 0.f;
};
