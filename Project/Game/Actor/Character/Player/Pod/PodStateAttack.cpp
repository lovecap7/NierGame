#include "PodStateAttack.h"
#include "PodStateIdle.h"
#include "PodStateGliding.h"
#include "Pod.h"
#include "../../../../Attack/BulletAttack.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/CSV/AttackData.h"

namespace
{
	const std::wstring kAttackAnim = L"Attack";
	const std::wstring kAttackData = L"NormalShot";
	//位置
	constexpr float kPodPosRight = -40.0f;
	constexpr float kPodPosBack = -40.0f;
	constexpr float kPodPosUp = 70.0f;
	//線形補間
	constexpr float kLerpRate = 0.5f;
}

PodStateAttack::PodStateAttack(std::weak_ptr<Actor> pod):
	PodStateBase(pod)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Pod>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kAttackAnim).c_str(), true);
	//攻撃データ
	m_attackData = owner->GetAttackData(kAttackData);
}

PodStateAttack::~PodStateAttack()
{
}

void PodStateAttack::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PodStateAttack::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Pod>(m_pOwner.lock());

	//滑空状態
	if (owner->IsGliding())
	{
		//滑空
		ChangeState(std::make_shared<PodStateGliding>(m_pOwner));
		return;
	}

	auto& input = Input::GetInstance();
	if (!input.IsPress("RB"))
	{
		//待機
		ChangeState(std::make_shared<PodStateIdle>(m_pOwner));
		return;
	}

	//プレイヤーの近くに移動
	Vector3 targetPos = owner->GetPlayerPos();
	//カメラの向き
	Vector3 dir = GetPodDir(owner);

	//今の座標
	Vector3 nowPos = owner->GetPos();

	//座標
	Vector3 nextPos = Vector3::Lerp(nowPos, GetPodPos(targetPos, dir, kPodPosUp, kPodPosRight, kPodPosBack), kLerpRate);
	owner->GetRb()->SetVec(nextPos - nowPos);

	//向きをカメラに合わせる
	owner->GetModel()->SetDir(dir.XZ());

	//カウント
	CountFrame();

	//発生フレームになったら弾を打つ
	if (m_attackData->m_startFrame >= m_frame)
	{
		//弾を打つ
		auto bullets = owner->GetBullets();
		for (auto bullet : bullets)
		{
			//活動中でない弾があるなら
			if (!bullet->IsActive())
			{
				//Activeにしつつフラグ等のリセット
				bullet->Reset(m_attackData->m_keepFrame);
				bullet->SetPos(owner->GetPos());
				bullet->SetMoveVec(dir * m_attackData->m_moveSpeed);
				owner->SetAttack(bullet);
				break;
			}
		}
		m_frame = 0.0f;
	}
	
}
