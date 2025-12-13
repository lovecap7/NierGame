#include "Boss4StateDeath.h"
#include "Boss4StateIdle.h"
#include "Boss4.h"
#include "../EnemyBase.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/CharaStatus.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/NormalEffect.h"
#include "../../../../Camera/CameraController.h"
#include "../../../../Camera/BossCamera.h"

namespace
{
	//アニメーション
	const std::wstring kDeath = L"Death";
	const std::wstring kTrans = L"Down";
	//エフェクト
	const std::wstring kDeadEff = L"Dead";
	const std::wstring kTransEff = L"Trans";

	//ふっとばす力
	constexpr float kMinSmashPower = 10.0f;
	constexpr float kMaxSmashPower = 20.0f;

	//カメラ距離
	constexpr float kCameraDistance = 300.0f;

	//変身フレーム
	constexpr float kTransFrame = 300.0f;
}

Boss4StateDeath::Boss4StateDeath(std::weak_ptr<Actor> enemy, bool isWait) :
	EnemyStateBase(enemy,isWait)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Boss4>(m_pOwner.lock());
	auto model = owner->GetModel();
	owner->SetCollState(CollisionState::Dead);

	//移動量リセット
	owner->GetRb()->SetMoveVec(Vector3::Zero());

	//無敵に
	owner->GetCharaStatus()->SetIsNoDamage(true);

	Vector3 dir = model->GetDir();
	if (owner->GetTargetInfo().m_isFound)
	{
		dir = owner->GetToTargetVec();
	}

	//第二形態なら
	if (owner->IsSecondPhase())
	{
		//死亡アニメーション
		model->SetAnim(owner->GetAnim(kDeath).c_str(), false);

		//カメラの作成
		CameraController::GetInstance().PushCamera(std::make_shared<BossCamera>(owner->GetCenterPos(), dir,
			kCameraDistance, owner->GetActorManager(), false));
	}
	//第一形態なら
	else
	{
		//変身
		model->SetAnim(owner->GetAnim(kTrans).c_str(), true);
		//変身エフェクト
		EffekseerManager::GetInstance().CreateEffect(owner->GetEffectPath(kTransEff), owner->GetPos());
	}
}

Boss4StateDeath::~Boss4StateDeath()
{
}

void Boss4StateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void Boss4StateDeath::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Boss4>(m_pOwner.lock());
	auto model = owner->GetModel();

	if (owner->IsSecondPhase())
	{
		//モデルのアニメーションが終わったら
		if (model->IsFinishAnim())
		{
			if (!owner->IsDelete())
			{
				//爆発
				EffekseerManager::GetInstance().CreateEffect(owner->GetEffectPath(kDeadEff), owner->GetPos());
			}
			//第二形態なら死亡
			owner->Delete();
		}
	}
	else
	{
		if (m_frame > kTransFrame)
		{
			//第一形態なら第二形態へ移行
			owner->ChangeSecondPhase();
			ChangeState(std::make_shared<Boss4StateIdle>(m_pOwner, m_isWait));
			return;
		}
	}

	++m_frame;
}
