#include "PodStateAttack.h"
#include "PodStateIdle.h"
#include "Pod.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"

namespace
{
	const char* kAnim = "RobotArmature|Shoot";

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
	owner->GetModel()->SetAnim(kAnim, true);
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
	Vector3 cameraDir = owner->GetCameraDir();

	//今の座標
	Vector3 nowPos = owner->GetPos();

	//座標
	Vector3 nextPos = Vector3::Lerp(nowPos, GetPodPos(targetPos, cameraDir, kPodPosUp, kPodPosRight, kPodPosBack), kLerpRate);
	owner->GetRb()->SetVec(nextPos - nowPos);

	//向きをカメラに合わせる
	owner->GetModel()->SetDir(cameraDir.XZ());
}
