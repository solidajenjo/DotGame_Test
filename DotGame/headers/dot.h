#pragma once
#include <vector>
#include <memory>

class Behaviour;
class Game;

typedef std::shared_ptr<Behaviour> BPtr;

class Dot
{
public:
	Dot(bool isLeft) : isLeft(isLeft) {};
	~Dot();

	void Update(const Game& game);

	void AddBehaviour(Behaviour* newBehaviour);

	bool isLeft;

private:

	std::vector<BPtr> behaviours;
};