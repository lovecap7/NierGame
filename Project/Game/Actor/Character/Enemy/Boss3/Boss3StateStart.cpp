#include "Boss3StateStart.h"
#include "Boss3StateIdle.h"
#include "Boss3.h"
#include "../EnemyBase.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/CharaStatus.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../Camera/BossStartCamera.h"
#include "../../../../Camera/CameraController.h"
namespace
{
	//アニメーション
	const std::wstring kStart = L"Start";
}

Boss3StateStart::Boss3StateStart(std::weak_ptr<Actor> enemy) :
	EnemyStateBase(enemy,false),
	m_isCreateCamera(false)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<EnemyBase>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kStart).c_str(), false);
	owner->SetCollState(CollisionState::Normal);
}

Boss3StateStart::~Boss3StateStart()
{
}

void Boss3StateStart::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void Boss3StateStart::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Boss3>(m_pOwner.lock());

	auto model = owner->GetModel();

	//カメラの作成
	if (!m_isCreateCamera)
	{
		CameraController::GetInstance().PushCamera(std::make_shared<BossStartCamera>(owner->GetCenterPos(), model->GetDir(), owner->GetActorManager()));
		m_isCreateCamera = true;
	}

	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		ChangeState(std::make_shared<Boss3StateIdle>(owner, m_isWait));
		return;
	}

	owner->GetRb()->SetMoveVec(Vector3::Zero());
}
