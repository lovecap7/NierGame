#include "PlayerStateBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../../General/Input.h"

PlayerStateBase::PlayerStateBase(std::weak_ptr<Actor>  player):
	CharacterStateBase(player)
{
}

PlayerStateBase::~PlayerStateBase()
{
}