#include "DebugPlayer.h"
#include "DebugWeapon.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Input.h"
#include "../../Camera/PlayerCamera.h"
#include "../../../Main/Application.h"
#include "../../../General/ShaderPostProcess.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include <cassert>

namespace
{
	constexpr float kSpeed = 20.0f;
	constexpr float kJumpPower = 15.0f;
	//アニメーション
	const char* kAnimNameIdle = "rig_deform|Idle_N";
	const char* kAnimNameIdleS = "rig_deform|Idle_S";
	const char* kAnimNameWalk = "rig_deform|Walk";
}

DebugPlayer::DebugPlayer(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager):
	Actor(actorData,Shape::Capsule,pActorManager),
	m_invincibleTime(0.0f)
{
}

DebugPlayer::~DebugPlayer()
{
}

void DebugPlayer::Init()
{
	//Physicsに登録
	Collidable::Init();
	//モデル
	m_model->SetAnim(kAnimNameIdle, true);
}

void DebugPlayer::Update()
{
	//マネージャーの参照チェック
	if (m_pActorManager.expired())return;
	auto actorManager = m_pActorManager.lock();
	//カメラの参照チェック
	if (actorManager->GetCamera().expired())return;
	auto camera = actorManager->GetCamera().lock();
	//入力取得
	auto& input = Input::GetInstance();

	//移動
	Vector3 vec = Vector3::Zero();
	vec.x = input.GetStickInfo().leftStickX;
	vec.z = -input.GetStickInfo().leftStickY;
	if(vec.SqMagnitude() > 0.0f)
	{
		vec = vec.Normalize();
		//移動
		m_model->SetAnim(kAnimNameWalk, true);
	}
	else
	{
		//停止
		m_model->SetAnim(kAnimNameIdle, true);
	}
	

	//カメラの向きに合わせて移動方向を変える
	vec = camera->GetRot() * vec;
	vec *= kSpeed;
	m_rb->SetMoveVec(vec);

	//モデルの向き
	m_model->SetDir(Vector2(vec.x, vec.z));


	//ジャンプ
	if (input.IsTrigger("A") && m_isFloor)
	{
		m_rb->m_vec.y = kJumpPower;
	}
	//タイムスケール
	if (input.IsTrigger("Y"))
	{
		m_rb->SetIsMyTimeScale(!m_rb->IsMyTimeScale());
		if (m_rb->IsMyTimeScale())
		{
			SetTimeScale(1.0f);
		}
	}
	//ジャスト回避
	if (input.IsTrigger("B"))
	{
		m_invincibleTime = 120.0f;
	}
	if (m_invincibleTime > 0.0f)
	{
		auto& app = Application::GetInstance();
		//無敵時間を減らす
		--m_invincibleTime;
		if (m_invincibleTime <= 0.0f)
		{
			app.GetPostProcess()->SubPostEffectState(ShaderPostProcess::PostEffectState::JustAvoid);
			m_invincibleTime = 0.0f;
			return;
		}
		//ジャスト状態
		app.GetPostProcess()->AddPostEffectState(ShaderPostProcess::PostEffectState::JustAvoid);
		app.GetPostProcess()->SetJustAvoidEffectTime(std::sin(m_invincibleTime / 120.0f));
	}

	if (input.IsTrigger("LB") && !m_pEnemy.expired())
	{
		if (camera->IsLockOn())
		{
			//ロックオン解除
			camera->EndLockOn();
		}
		else
		{
			//ロックオン
			camera->StartLockOn(m_pEnemy);
		}
	}
	//カメラに位置を渡す
	camera->SetPlayerPos(m_rb->GetNextPos());
	camera->SetPlayerVec(m_rb->GetMoveVec());
	//モデルの更新
	m_model->Update();
}

void DebugPlayer::Draw() const
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
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(), 50.0f, 16, 0x00ff00, 0, true);
	//タイムスケール表示
	if (m_rb->IsMyTimeScale())
	{
		DrawString(0, 60, L"PlayerのTimeScaleを使用中", 0xffffff);
	}
	else
	{
		DrawString(0, 60, L"PlayerのTimeScaleを使用していない", 0xffffff);
	}
	DrawFormatString(0, 80, 0xffffff, L"PlayerのTimeScale = %.1f", m_rb->m_myTimeScale);
	//モデルの描画
	m_model->Draw();
}

void DebugPlayer::End()
{
	//Physicsから解除
	Collidable::End();
	//モデルの解放
	m_model->End();
}

void DebugPlayer::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void DebugPlayer::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->m_collHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();
}

void DebugPlayer::SetWeapon(const std::weak_ptr<DebugWeapon> weapon)
{
	m_weapon = weapon;
	if (!m_weapon.expired())
	{
		m_weapon.lock()->SetOwnerHandle(m_model->GetModelHandle());
		m_weapon.lock()->SetFrameIndex(295);
		m_weapon.lock()->SetScale(0.12f);
	}
}

void DebugPlayer::JustAvoid()
{
	m_invincibleTime = 2.0f;
}
