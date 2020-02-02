#include <dot.h>
#include <behaviours.h>

void Dot::Clear()
{
	for (auto b : behaviours)
		delete b;
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
	behaviours.push_back(newBehaviour);
}
