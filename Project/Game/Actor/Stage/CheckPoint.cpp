#include "CheckPoint.h"
#include "../ActorManager.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/NormalEffect.h"
#include "../../../General/AssetManager.h"
#include "../Character/Player/Player.h"

CheckPoint::CheckPoint(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData, Shape::Sphere, pActorManager)
{
}

CheckPoint::~CheckPoint()
{
}

void CheckPoint::Init()
{
	Collidable::Init();
}

void CheckPoint::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (other->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(other)->SetRespawnPos(m_rb->GetPos());
		m_isDelete = true;
	}
}

void CheckPoint::Draw() const
{
#if _DEBUG
	DrawSphere3D(m_rb->GetPos().ToDxLibVector(), m_actorData->GetCollRadius(), 32, 0x00ffff, 0x00ffff, false);
#endif
}

void CheckPoint::End()
{
	Collidable::End();
}
