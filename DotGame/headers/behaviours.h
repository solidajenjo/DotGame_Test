#pragma once
#include <stdint.h>
#include <memory>

class Texture;
class Game;

class Behaviour
{
public :
	Behaviour() {};

	virtual ~Behaviour() {}
	virtual void Update(const Game& game) = 0;
};

class SpriteBehaviour : public Behaviour
{
public:
	SpriteBehaviour(std::shared_ptr<Texture> t, uint16_t x, uint16_t y) : texture(t), x(x), y(y) {};

	void Update(const Game& game) override;

private:
	std::shared_ptr<Texture> texture;
	uint16_t x, y;
};

