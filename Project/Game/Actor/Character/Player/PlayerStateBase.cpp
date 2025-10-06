#include "PlayerStateBase.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"

PlayerStateBase::PlayerStateBase(std::weak_ptr<Actor>  player):
	CharacterStateBase(player)
{
}

PlayerStateBase::~PlayerStateBase()
{
}

void PlayerStateBase::RandAttackVC()
{
	auto player = std::dynamic_pointer_cast<Player>(m_owner.lock());
	switch (MyMath::GetRand(1,3))
	{
	case 1:
		player->CharacterVC("Attack1");
		break;
	case 2:
		player->CharacterVC("Attack2");
		break;
	case 3:
		player->CharacterVC("Attack3");
		break;
	default:
		break;
	}
}

Vector3 PlayerStateBase::GetForwardVec(const std::weak_ptr<GameCamera> camera)
{
	Vector3 rV = { 0.0f,0.0f,0.0f, };
	//プレイヤー
	auto player = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//入力
	Vector3 stickVec = player->GetPlayerStickVec().XZ();
	if (stickVec.Magnitude() < 0.0f)
	{
		return rV;
	}
	//カメラの向きにあわせる
	//カメラの向き
	Vector3 cameraDir = camera.lock()->GetDir();
	cameraDir.y = 0.0f;
	if (cameraDir.Magnitude() > 0.0f)
	{
		cameraDir = cameraDir.Normalize();
	}
	//回転マトリクスを作成
	Matrix4x4 cameraM = Matrix4x4::LookAt(cameraDir, Vector3::Up());
	Matrix4x4 stickM = Matrix4x4::LookAt(stickVec, Vector3::Up());
	//ベクトルにかける(回転)
	Vector3 moveVec = Vector3::Forward();
	moveVec = stickM * cameraM * moveVec;
	moveVec.y = 0.0f; //Y軸は無視
	if (moveVec.Magnitude() > 0.0f)
	{
		rV = moveVec.Normalize();
	}
	return rV;
}