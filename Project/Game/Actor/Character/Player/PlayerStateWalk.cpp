#include "PlayerStateWalk.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	constexpr float kSpeed = 5.0f;
	const char* kAnimNameWalk = "Player|Jog_N";
}

PlayerStateWalk::PlayerStateWalk(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//•à‚«ó‘Ô
	auto owner = std::dynamic_pointer_cast<Player>(m_owner.lock());
	owner->GetModel()->SetAnim(kAnimNameWalk, true);
	owner->SetCollState(CollisionState::Move);
}

PlayerStateWalk::~PlayerStateWalk()
{
}

void PlayerStateWalk::Init()
{
	//Ÿ‚Ìó‘Ô‚ğ©•ª‚Ìó‘Ô‚ğ“ü‚ê‚é
	ChangeState(shared_from_this());
}

void PlayerStateWalk::Update()
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
	vec *= kSpeed;
	owner->GetRb()->SetMoveVec(vec);

	//ƒ‚ƒfƒ‹‚ÌŒü‚«
	owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
}
