#include "DebugPlayer.h"
#include "DebugWeapon.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Input.h"
#include "../../Camera/PlayerCamera.h"
#include "../../../Main/Application.h"
#include "../../../General/ShaderPostProcess.h"
#include "../../../General/Model.h"
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

DebugPlayer::DebugPlayer():
	Actor(Shape::Sphere),
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
	//初期化
	m_rb->m_pos = Vector3::Zero();
	m_rb->m_vec = Vector3::Zero();
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false, true);
	//コライダーの設定
	auto sphereCollider = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	sphereCollider->SetRadius(50.0f);
	//モデルのロード
	int handle = MV1LoadModel(L"Data/Model/Player/Player.mv1");
	assert(handle != -1);
	m_model = std::make_shared<Model>(handle, m_rb->m_pos.ToDxLibVector());
	m_model->SetScale(VGet(2.0f, 2.0f, 2.0f));
	m_model->SetAnim(kAnimNameIdle, true);
}

void DebugPlayer::Update()
{
	if (m_pCamera.expired())return;
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
	vec = m_pCamera.lock()->GetRot() * vec;
	vec *= kSpeed;
	m_rb->SetMoveVec(vec);

	//モデルの向き
	m_model->SetDir(Vector2(vec.x, vec.z));


	//ジャンプ
	if (input.IsTrigger("B"))
	{
		if (m_rb->m_pos.y <= 0.0f)
		{
			m_rb->m_vec.y = kJumpPower;
		}
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
	if (input.IsTrigger("A"))
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
		auto camera = m_pCamera.lock();
		if (camera->IsLockOn())
		{
			//ロックオン解除
			camera->EndLockOn();
		}
		else
		{
			//ロックオン
			m_pCamera.lock()->StartLockOn(m_pEnemy);
		}
	}
	//カメラに位置を渡す
	m_pCamera.lock()->SetPlayerPos(m_rb->GetNextPos());
	//モデルの更新
	m_model->Update();
}

void DebugPlayer::Draw() const
{
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
	if (m_rb->GetNextPos().y < 0.0f)
	{
		m_rb->m_pos.y = 0.0f;
		m_rb->m_vec.y = 0.0f;
	}
	//Rigidbodyの位置を更新
	m_rb->SetPos(m_rb->GetNextPos());
	//モデルの位置を更新
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
