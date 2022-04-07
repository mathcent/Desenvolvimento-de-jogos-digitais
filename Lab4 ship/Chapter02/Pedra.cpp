#include "AnimSpriteComponent.h"
#include "Game.h"
#include "Pedra.h"

Pedra::Pedra(Game* game)
	:Actor(game)
	,mRightSpeed(-600.0f)
	,mDownSpeed(0.0f)

{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Pedra.png")
	};
	asc->SetAnimTextures(anims);


}

void Pedra::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;

	SetPosition(pos);
}