#include "PlayerStateStart.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"
#include "../../../UI/UIManager.h"

namespace
{
	//アニメーション
	const std::wstring kStart = L"Start";
}

PlayerStateStart::PlayerStateStart(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());

	//武器を持つ
	owner->HaveLightSword();

	//アニメーション
	owner->GetModel()->SetAnim(owner->GetAnim(kStart).c_str(), false);
	owner->SetCollState(CollisionState::Normal);

	//ステータス
	auto status = owner->GetCharaStatus();
	//無敵
	status->SetIsNoDamage(true);
	//UI非表示
	UIManager::GetInstance().SetIsDraw(false);
	//強制待機状態にする(ポッドの射撃を不可能に)
	Wait();
}

PlayerStateStart::~PlayerStateStart()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto status = owner->GetCharaStatus();
	//無敵解除
	status->SetIsNoDamage(false);
	//UI表示
	UIManager::GetInstance().SetIsDraw(true);
	//通常状態に戻す
	Operate();
}
void PlayerStateStart::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateStart::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());

	//武器を持つ
	owner->HaveLightSword();

	//アニメーション終了
	if (owner->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
		return;
	}

	//移動量リセット
	owner->GetRb()->SetMoveVec(Vector3::Zero());
}
