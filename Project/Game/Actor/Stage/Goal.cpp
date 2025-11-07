#include "Goal.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../Character/Player/Player.h"

Goal::Goal(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData, Shape::Sphere, pActorManager)
{
}

Goal::~Goal()
{
}

void Goal::Init()
{
	if (m_isThrough)return;
	Collidable::Init();
}

void Goal::Update()
{
	
}

void Goal::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (other->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(other)->SetIsGoal(true);
	}
}

void Goal::Draw() const
{
	DrawSphere3D(m_rb->GetPos().ToDxLibVector(), m_actorData->GetCollRadius(), 32, 0x00ffff, 0x00ffff, false);
}

void Goal::Complete()
{
}

void Goal::End()
{
	if (m_isThrough)return;
	Collidable::End();
}
