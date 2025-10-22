#include "EnemyBase.h"

EnemyBase::EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager):
	CharacterBase(actorData, charaStatusData, Shape::Capsule, pActorManager)
{
}

EnemyBase::~EnemyBase()
{
}
