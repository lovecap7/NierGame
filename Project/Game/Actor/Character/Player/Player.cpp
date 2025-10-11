#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../ActorManager.h"
#include "../../../Camera/PlayerCamera.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//ジャンプの最大数
	constexpr int kMaxJumpNum = 2;
}

Player::Player(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	CharacterBase(actorData,charaStatusData,Shape::Capsule,pActorManager),
	m_jumpNum(0)
{
	
}

Player::~Player()
{
}

void Player::Init()
{
	//Physicsに登録
	Collidable::Init();
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->m_collRadius);
}


void Player::Update()
{
	//入力の取得
	auto& input = Input::GetInstance();

	//タイムスケール
	if (input.IsTrigger("Y"))
	{
		m_rb->SetIsMyTimeScale(!m_rb->IsMyTimeScale());
		if (m_rb->IsMyTimeScale())
		{
			SetTimeScale(0.5f);
		}
		else
		{
			SetTimeScale(1.0f);
		}
	}

	//状態に合わせた更新
	m_state->Update();
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();

	//状態のリセット
	m_charaStatus->InitHitState();
}

void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
	//デバッグ
	if (other->GetGameTag() == GameTag::Attack)
	{
		//ダメージを受ける
		m_charaStatus->OnDamage(10, 100, CharaStatus::AttackWeight::Heavy);
		if (m_charaStatus->IsHitReaction())
		{
			Vector3 knockBack = m_rb->m_pos - std::dynamic_pointer_cast<Actor>(other)->GetNextPos();
			knockBack = knockBack.Normalize() * 2.0f;
			knockBack.y = 0.0f;
			m_rb->m_vec = knockBack;
			printf("ダメージを受けた！！ %d\n", 10);
		}
	}
}

void Player::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false//地面にいると塗りつぶされる
	);
#endif
	m_model->Draw();
}

void Player::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->m_collHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();
	//ジャンプ回数リセット
	if (m_isFloor)
	{
		m_jumpNum = 0;
	}
	//カメラに位置を渡す
	if (!GetPlayerCamera().expired())
	{
		auto camera = GetPlayerCamera().lock();
		camera->SetPlayerPos(m_rb->m_pos);
		camera->SetPlayerVec(m_rb->GetMoveVec());
	}
}


void Player::End()
{
	m_model->End();
	Collidable::End();
}

Quaternion Player::GetCameraRot() const
{
	Quaternion q = Quaternion::IdentityQ();
	if (!m_pActorManager.expired())
	{
		auto actorM = m_pActorManager.lock();
		if (!actorM->GetCamera().expired())
		{
			q = actorM->GetCamera().lock()->GetRot();
		}
	}
	return q;
}

void Player::AddJumpNum()
{
	++m_jumpNum;
	m_jumpNum = MathSub::ClampInt(m_jumpNum, 0, kMaxJumpNum);
}

bool Player::IsJumpable() const
{
	return m_jumpNum < kMaxJumpNum;
}

bool Player::IsFall() const
{
	return m_rb->m_vec.y < 0.0f;
}

std::weak_ptr<PlayerCamera> Player::GetPlayerCamera() const
{
	std::weak_ptr<PlayerCamera> camera;
	if (!m_pActorManager.expired())
	{
		auto actorM = m_pActorManager.lock();
		if (!actorM->GetCamera().expired())
		{
			camera = actorM->GetCamera();
		}
	}
	return camera;
}
