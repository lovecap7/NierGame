#include "AttackBase.h"
#include "../Actor/Character/CharacterBase.h"
#include "../../General/CSV/AttackData.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/TorusCollider.h"
#include "../../General/CharaStatus.h"
#include "../../Main/Application.h"

AttackBase::AttackBase(Shape shape, std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner) :
	Collidable(shape),
	m_pOwner(pOwner)
{
	if (!attackData)return;
	m_attackPower = attackData->m_attackPower;
	m_attackWeight = attackData->m_attackWeight;
	m_knockBackPower = attackData->m_knockBackPower; 
	m_knockBackV = attackData->m_verticalPower;
	m_keepFrame = attackData->m_keepFrame;

	//コライダーの設定
	switch (shape)
	{
	case Shape::Sphere:
	{
		auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
		coll->SetRadius(attackData->m_radius);
		break;
	}
	case Shape::Capsule:
	{
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		coll->SetRadius(attackData->m_radius);
		break;
	}
	case Shape::Torus:
	{
		auto coll = std::dynamic_pointer_cast<TorusCollider>(m_collisionData);
		coll->SetRadius(attackData->m_radius);
		coll->SetRange(attackData->m_length);
		break;
	}
	default:
		break;
	};
	AllSetting(Priority::Static,GameTag::Attack, false,true, false);
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init()
{
	Collidable::Init();
}

void AttackBase::Update()
{
	//持続フレームを減らす
	m_keepFrame -= Application::GetInstance().GetTimeScale();
	//持続フレームが0になったら削除
	if (m_keepFrame < 0 || m_pOwner.expired())
	{
		return; //何もしない
	}
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_pOwner.expired())return;
	if (!other)return;

	auto ownerColl = m_pOwner.lock();
	auto otherColl = other;

	auto otherTag = otherColl->GetGameTag();
	//キャラクターなのかをチェック
	if (otherTag == GameTag::Player || otherTag == GameTag::Enemy)
	{
		//自分と同じ種類なら無視
		if (otherTag == ownerColl->GetGameTag())return;

		std::shared_ptr<Actor> otherActor = std::dynamic_pointer_cast<Actor>(otherColl);
		bool isFind = false;
		//IDがすでに記録されているか確認
		for (auto id : m_hitId)
		{
			if (id == otherActor->GetID())
			{
				isFind = true;
				break;
			}
		}
		//攻撃成功
		if (!isFind)
		{
			//相手
			auto otherStatus = std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetCharaStatus();

			//攻撃力
			int at = ownerColl->GetCharaStatus()->GetAT();

			//ダメージを与える
			otherStatus->OnDamage(m_attackPower, at, m_attackWeight);
		}
	}
}

void AttackBase::End()
{
	Collidable::End();
}

bool AttackBase::IsDelete()
{
	return m_keepFrame <= 0.0f;
}

void AttackBase::Delete()
{
	m_keepFrame = 0.0f;
}

void AttackBase::ResetHitID()
{
	if (m_hitId.size() <= 0) return;
	m_hitId.clear();
}
