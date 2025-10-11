#include "PlayerStateJustAvoid.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../Main/Application.h"
#include "../../../../General/ShaderPostProcess.h"

namespace
{
	const char* kAnimNameJustAvoid = "Player|JustAvoid";
}

PlayerStateJustAvoid::PlayerStateJustAvoid(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->SetCollState(CollisionState::Move);
	//ジャスト回避
	auto model = owner->GetModel();
	model->SetAnim(kAnimNameJustAvoid, false);
	//スローモーション
	auto& app = Application::GetInstance();

	//シェーダー
	app.SetTimeScale(0.5f);
	app.GetPostProcess()->SetJustAvoidEffectTime(model->GetTotalAnimFrame());
	app.GetPostProcess()->AddPostEffectState(ShaderPostProcess::PostEffectState::JustAvoid);

	//通常速度
	owner->SetTimeScale(1.0f);
	owner->EnableIsMyScale();
}

PlayerStateJustAvoid::~PlayerStateJustAvoid()
{
	//元に戻す
	Application::GetInstance().SetTimeScale(1.0f);
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//無敵
	owner->SetNoDamageFrame(40.0f);
	//無効
	owner->DisableIsMyScale();
}

void PlayerStateJustAvoid::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateJustAvoid::Update()
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//無敵
	owner->GetCharaStatus()->SetIsNoDamage(true);

	//アニメーションが終わったら
	auto model = owner->GetModel();
	if (model->IsFinishAnim())
	{
		auto& input = Input::GetInstance();
		if (input.GetStickInfo().IsLeftStickInput())
		{
			//走る
			ChangeState(std::make_shared<PlayerStateMoving>(m_owner, true));
			return;
		}
		else
		{
			//待機
			ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
			return;
		}
	}
}
