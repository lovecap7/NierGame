#pragma once
#include "../CharacterBase.h"
class ActorData;
class CharaStatusData;
class ActorManager;
class EnemyBase abstract:
    public CharacterBase
{
public:
	EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~EnemyBase();

};

