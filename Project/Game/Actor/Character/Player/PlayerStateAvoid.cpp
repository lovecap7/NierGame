#include "PlayerStateAvoid.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateJustAvoid.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"

namespace
{
	constexpr float kSpeedDif = 10.0f;
	constexpr float kLerpSpeedRate = 0.1f;
	const char* kAnimNameForwardAvoid = "Player|AvoidForward";
	const char* kAnimNameBackAvoid = "Player|AvoidBack";
	//ジャスト回避フレーム
	constexpr float kJustFrame = 15.0f;
}

PlayerStateAvoid::PlayerStateAvoid(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_avoidDir(),
	m_speed(0.0f),
	m_endSpeed(0.0f)
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->SetCollState(CollisionState::Move);

	auto& input = Input::GetInstance();
	//移動
	Vector3 vec = Vector3::Zero();
	vec.x = input.GetStickInfo().leftStickX;
	vec.z = -input.GetStickInfo().leftStickY;
	//下がる
	bool isBack = false;
	//移動方向があるなら
	if (vec.SqMagnitude() > 0.0f)
	{
		vec = vec.Normalize();
	}
	//ない場合は下がる
	else
	{
		isBack = true;
		vec = Vector3::Back();
	}
	//カメラの向きに合わせて移動方向を変える
	vec = owner->GetCameraRot() * vec;
	//回避方向設定
	m_avoidDir = vec;

	//前進か下がるかでアニメーション切り替え
	if (!isBack)
	{
		owner->GetModel()->SetAnim(kAnimNameForwardAvoid, false);
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
	}
	else
	{
		owner->GetModel()->SetAnim(kAnimNameBackAvoid, false);
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(-vec.x, -vec.z));
	}

	auto status = owner->GetCharaStatus();
	//速度
	float baseSpeed = status->GetMS();
	m_endSpeed = baseSpeed + kSpeedDif;
	float startSpeed = MathSub::ClampFloat(baseSpeed - kSpeedDif,0, m_endSpeed);
	m_speed = startSpeed;

	//空中にいるなら重力の影響を受けない
	if (!owner->IsFloor())
	{
		auto rb = owner->GetRb();
		rb->SetIsGravity(false);
		rb->SetVecY(0.0f);
		//回避不可能(ジャンプをするか地面に付くと可能に)
		owner->SetIsAvoidable(false);
	}

	//無敵
	status->SetIsNoDamage(true);
}

PlayerStateAvoid::~PlayerStateAvoid()
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetRb()->SetIsGravity(true);
}

void PlayerStateAvoid::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateAvoid::Update()
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = owner->GetModel();

	//ジャスト回避フレーム内で攻撃を受けたら
	CountFrame();
	if (m_frame <= kJustFrame && owner->GetCharaStatus()->IsHit())
	{
		//ジャスト回避
		ChangeState(std::make_shared<PlayerStateJustAvoid>(m_owner));
		return;
	}

	if (model->IsFinishAnim())
	{
		//無敵解除
		owner->GetCharaStatus()->SetIsNoDamage(false);
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
	m_speed = MathSub::Lerp(m_speed, m_endSpeed, kLerpSpeedRate);
	Vector3 vec = m_avoidDir * m_speed;
	//移動
	owner->GetRb()->SetMoveVec(vec);
}
