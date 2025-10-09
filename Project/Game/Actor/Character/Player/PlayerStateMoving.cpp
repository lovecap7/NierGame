#include "PlayerStateMoving.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	constexpr float kWalkSpeed = 10.0f;
	constexpr float kRunSpeed = 20.0f;
	const char* kAnimNameJog = "Player|Jog_N";
	const char* kAnimNameRun = "Player|Run";
}

PlayerStateMoving::PlayerStateMoving(std::weak_ptr<Actor> player, bool isDash) :
	PlayerStateBase(player),
	m_speed(0.0f)
{
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//•à‚«
	if (!isDash)
	{
		owner->GetModel()->SetAnim(kAnimNameJog, true);
		m_speed = kWalkSpeed;
	}
	//‘–‚è
	else
	{
		owner->GetModel()->SetAnim(kAnimNameRun, true);
		m_speed = kRunSpeed;
	}
	owner->SetCollState(CollisionState::Move);
}

PlayerStateMoving::~PlayerStateMoving()
{
}

void PlayerStateMoving::Init()
{
	//Ÿ‚Ìó‘Ô‚ğ©•ª‚Ìó‘Ô‚ğ“ü‚ê‚é
	ChangeState(shared_from_this());
}

void PlayerStateMoving::Update()
{
	if (m_owner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto& input = Input::GetInstance();

	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//‘Ò‹@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	if (input.IsTrigger("B"))
	{
		//‰ñ”ğ
		ChangeState(std::make_shared<PlayerStateAvoid>(m_owner));
		return;
	}
	//ˆÚ“®
	Vector3 vec = Vector3::Zero();
	vec.x = input.GetStickInfo().leftStickX;
	vec.z = -input.GetStickInfo().leftStickY;
	if (vec.SqMagnitude() > 0.0f)
	{
		vec = vec.Normalize();
	}
	//ƒJƒƒ‰‚ÌŒü‚«‚É‡‚í‚¹‚ÄˆÚ“®•ûŒü‚ğ•Ï‚¦‚é
	vec = owner->GetCameraRot() * vec;
	vec *= m_speed;
	owner->GetRb()->SetMoveVec(vec);

	//ƒ‚ƒfƒ‹‚ÌŒü‚«
	owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
}
