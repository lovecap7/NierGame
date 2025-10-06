#include "StageObject.h"
#include "../../../General/Collision/ColliderBase.h"

StageObject::StageObject(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager):
	Actor(actorData,Shape::Polygon,pActorManager)
{
}

StageObject::~StageObject()
{
}

void StageObject::Init()
{
}

void StageObject::Update()
{
}

void StageObject::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void StageObject::Draw() const
{
}

void StageObject::Complete()
{
}

void StageObject::End()
{
}

void StageObject::SetIsThrough(bool isThrough)
{
}
