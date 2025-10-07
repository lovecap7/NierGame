#pragma once
#include "../CharacterStateBase.h"
#include <memory>
#include "../../../../General/Math/MyMath.h"
#include <string>
class Actor;
class ActorManager;
class PlayerStateBase abstract:
	public CharacterStateBase
{
public:
	PlayerStateBase(std::weak_ptr<Actor> player);
	virtual ~PlayerStateBase();
};

