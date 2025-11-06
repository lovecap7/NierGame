#include "AttackBase.h"
#include "../Actor/Character/CharacterBase.h"
#include "../Actor/Character/Enemy/EnemyBase.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/TorusCollider.h"
#include "../../General/CharaStatus.h"
#include "../../General/Collision/Rigidbody.h"
#include "../../Main/Application.h"

AttackBase::AttackBase(Shape shape, std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner) :
	Collidable(shape),
	m_pOwner(pOwner),
	m_oriPos(Vector3::Zero())
{
	if (!attackData)return;
	m_attackPower = attackData->GetAttackPower();
	m_attackWeight = attackData->GetAttackWeight();
	m_knockBackPower = attackData->GetKnockBackPower(); 
	m_knockBackV = attackData->GetVerticalPower();
	m_keepFrame = attackData->GetKeepFrame();
	m_originPosData = attackData->GetAttackOriginPos();
	m_hitStopFrame = attackData->GetHitStopFrame();
	m_hitStopShakePower = attackData->GetHitStopShakePower();
	m_isHit = false;
	m_isRequestHitStop = false;

	//コライダーの設定
	switch (shape)
	{
	case Shape::Sphere:
	{
		auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
		coll->SetRadius(attackData->GetRadius());
		break;
	}
	case Shape::Capsule:
	{
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		coll->SetRadius(attackData->GetRadius());
		break;
	}
	case Shape::Torus:
	{
		auto coll = std::dynamic_pointer_cast<TorusCollider>(m_collisionData);
		coll->SetRadius(attackData->GetRadius());
		coll->SetRange(attackData->GetLength());
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
	//ヒットリセット
	m_isHit = false;
	m_isRequestHitStop = false;
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_pOwner.expired())return;
	if (!other)return;

	auto ownerColl = m_pOwner.lock();

	auto otherTag = other->GetGameTag();
	//キャラクターなのかをチェック
	if (otherTag == GameTag::Player || otherTag == GameTag::Enemy)
	{
		//自分と同じ種類なら無視
		if (otherTag == ownerColl->GetGameTag())return;
		//敵なら
		if (otherTag == GameTag::Enemy)
		{
			//活動中でないなら攻撃は喰らわない
			if (!std::dynamic_pointer_cast<EnemyBase>(other)->IsActive())
			{
				return;
			}
		}

		auto otherColl = std::dynamic_pointer_cast<CharacterBase>(other);
		
		//ID
		int otherID = otherColl->GetID();

		//IDがすでに記録されているか確認
		if (m_hitId.contains(otherID))
		{
			//すでに当たっている
			return; 
		}
		else
		{
			//まだ当たっていない
			//IDを覚える
			m_hitId.insert(otherID);

			//相手
			auto otherStatus = otherColl->GetCharaStatus();

			//攻撃力
			int at = ownerColl->GetCharaStatus()->GetAT();

			//ダメージを与える
			otherStatus->OnDamage(m_attackPower, at, m_attackWeight);

			//のけぞるか
			if (m_rb && otherStatus->IsHitReaction())
			{
				Vector3 attackPos = m_oriPos;
				//ノックバックを与える
				Vector3 knockBack = otherStatus->GetKnockBack(otherColl->GetPos(), attackPos, m_knockBackPower, m_knockBackV);
				otherColl->GetRb()->SetVec(knockBack);
			}
			//無敵じゃないなら
			if (!otherStatus->IsNoDamage())
			{
				//ヒットストップを行う
				m_isRequestHitStop = true;
			}

			//当たった
			m_isHit = true;
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
