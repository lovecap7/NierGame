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

DebugEnemy::DebugEnemy(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData,Shape::Sphere, pActorManager)
{
}

DebugEnemy::~DebugEnemy()
{
}

void DebugEnemy::Init()
{
	//Physicsに登録
	Collidable::Init();
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
	//Rigidbodyの位置を更新
	m_rb->SetPos(m_rb->GetNextPos());
}
