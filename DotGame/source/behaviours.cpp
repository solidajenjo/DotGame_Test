#include <behaviours.h>
#include <game.h>
#include <modules.h>
#include <cassert>

void SpriteBehaviour::Update(const Game& game)
{
	auto render = game.GetModule<RenderModule>("renderer");
	assert(render);

	render->RenderTexture(*texture, x, y);
}
