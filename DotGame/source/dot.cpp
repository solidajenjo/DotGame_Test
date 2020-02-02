#include <dot.h>
#include <behaviours.h>

Dot::~Dot()
{
	behaviours.clear();
}

void Dot::Update(const Game& game)
{
	for (auto b : behaviours)
	{
		b->Update(game);
	}
}

void Dot::AddBehaviour(Behaviour* newBehaviour)
{
	BPtr b(newBehaviour);
	behaviours.push_back(b);
}
