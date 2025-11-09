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

namespace
{
	const std::wstring kCheckPointBeforePath = L"CheckPointBefore";
	const std::wstring kCheckPointAfterPath = L"CheckPointAfter";
}

CheckPoint::CheckPoint(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData, Shape::Sphere, pActorManager),
	m_isPass(false)
{
}

CheckPoint::~CheckPoint()
{
	if (m_effect.expired())return;
	m_effect.lock()->Delete();
}

void CheckPoint::Init()
{
	//エフェクト
	m_effect = EffekseerManager::GetInstance().CreateEffect(kCheckPointBeforePath, m_actorData->GetPos());
	if (m_isThrough)return;
	Collidable::Init();
}

void CheckPoint::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (other->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(other)->SetRespawnPos(m_rb->GetPos());
		m_isDelete = true;
		m_isPass = true;
	}
}

void CheckPoint::Draw() const
{
#if _DEBUG
	DrawSphere3D(m_rb->GetPos().ToDxLibVector(), m_actorData->GetCollRadius(), 32, 0x00ffff, 0x00ffff, false);
#endif
}

void CheckPoint::Complete()
{
	//通過したのなら
	if (m_isPass)
	{
		//エフェクトを削除して通過したことがわかるエフェクトを出す
		if (m_effect.expired())return;
		m_effect.lock()->Delete();
		EffekseerManager::GetInstance().CreateEffect(kCheckPointAfterPath, m_actorData->GetPos());
	}
}

void CheckPoint::End()
{
	Collidable::End();
}
