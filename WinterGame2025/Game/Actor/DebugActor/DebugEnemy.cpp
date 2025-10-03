#include "DebugEnemy.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Input.h"
#include "../../../Main/Application.h"

namespace
{
	constexpr float kSpeed = 500.0f;
	constexpr float kJumpPower = 400.0f;
}

DebugEnemy::DebugEnemy() :
	Actor(Shape::Sphere)
{
}

DebugEnemy::~DebugEnemy()
{
}

void DebugEnemy::Init()
{
	//Physicsに登録
	Collidable::Init();
	//初期化
	m_rb->m_pos = Vector3(100.0f,20.0f,1000.0f);
	m_rb->m_vec = Vector3::Zero();
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false, true);
	//コライダーの設定
	auto sphereCollider = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	sphereCollider->SetRadius(50.0f);
}

void DebugEnemy::Update()
{
	//入力取得
	auto& input = Input::GetInstance();
	//移動
	Vector3 vec = Vector3::Zero();
	//タイムスケールを変更
	if (input.IsTrigger("Y"))
	{
		m_rb->m_myTimeScale = 0.5f;
		m_rb->SetIsMyTimeScale(!m_rb->IsMyTimeScale());
	}
}

void DebugEnemy::Draw() const
{
	DrawSphere3D(VGet(m_rb->m_pos.x, m_rb->m_pos.y, m_rb->m_pos.z), 50.0f, 16, 0xff0000, 0, true);
	//タイムスケール表示
	if (m_rb->IsMyTimeScale())
	{
		DrawString(0, 100, L"EnemyのTimeScaleを使用中", 0xffffff);
	}
	else
	{
		DrawString(0, 100, L"EnemyのTimeScaleを使用していない", 0xffffff);
	}
	DrawFormatString(0, 120, 0xffffff, L"EnemyのTimeScale = %.1f", m_rb->m_myTimeScale);
}

void DebugEnemy::End()
{
	//Physicsから解除
	Collidable::End();
}

void DebugEnemy::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void DebugEnemy::Complete()
{
	if (m_rb->GetNextPos().y < 0.0f)
	{
		m_rb->m_pos.y = 0.0f;
		m_rb->m_vec.y = 0.0f;
	}
	//Rigidbodyの位置を更新
	m_rb->SetPos(m_rb->GetNextPos());
}
