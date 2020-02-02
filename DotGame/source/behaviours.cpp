#include <behaviours.h>
#include <game.h>
#include <modules.h>
#include <cassert>
#include <dot.h>
#include <cmath>

void SpriteBehaviour::Update(const Game& game)
{
	auto render = game.GetModule<RenderModule>("renderer");
	assert(render);

	render->RenderTexture(*texture, x, y);
}


void TranslateBehaviour::Update(const Game& game)
{
	if (done)
		return;

	auto sprite = (*owner).GetBehaviour<SpriteBehaviour>(BehaviourTypes::SPRITE);	
	auto logic = game.GetModule<GameLogicModule>("gameLogic");
	assert(sprite && logic);
	timePassed += logic->deltaTime;
	if (timePassed > time)
	{
		done = true;
		sprite->SetPosition(x1, y1);
	}
	else
	{
		float delta = timePassed / time;
		uint16_t x = LERP(x0, x1, delta);
		uint16_t y = LERP(y0, y1, delta);
		sprite->SetPosition(x, y);
	}	
}
