#pragma once
#include <vector>
#include <memory>
#include <behaviours.h>

class Game;

class Dot
{
public:
	Dot(bool isLeft) : isLeft(isLeft) {};
	void Clear();

	template <class T>
	T* GetBehaviour(Behaviour::BehaviourTypes type) const
	{
		for (auto b : behaviours)
		{
			if (b->type == type)
				return (T*)b;
		}
		return nullptr;
	}

	void Update(const Game& game);

	void AddBehaviour(Behaviour* newBehaviour);

	bool isLeft;

private:

	std::vector<Behaviour*> behaviours;
};