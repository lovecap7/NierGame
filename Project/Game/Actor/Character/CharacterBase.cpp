#include "CharacterBase.h"
#include "../../../General/Model.h"

CharacterBase::CharacterBase(std::shared_ptr<ActorData> actorData, Shape shape, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData,shape,pActorManager)
{
}