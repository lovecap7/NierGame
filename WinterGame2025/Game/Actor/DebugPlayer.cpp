#include "DebugPlayer.h"
#include "../../General/Collision/Rigidbody.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Input.h"
#include "../Camera/Camera.h"

DebugPlayer::DebugPlayer():
	Actor(Shape::Sphere)
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
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false, false);
	//コライダーの設定
	auto sphereCollider = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	sphereCollider->SetRadius(50.0f);
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
	}
	//カメラの向きに合わせて移動方向を変える
	vec = m_pCamera.lock()->GetRot() * vec;
	vec *= 2.0f;
	m_rb->SetVec(vec);
}

void DebugPlayer::Draw() const
{
	DrawSphere3D(VGet(m_rb->m_pos.x, m_rb->m_pos.y, m_rb->m_pos.z), 50.0f, 16, 0x00ff00, 0, true);
}

void DebugPlayer::End()
{
	//Physicsから解除
	Collidable::End();
}

void DebugPlayer::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void DebugPlayer::Complete()
{
	//Rigidbodyの位置を更新
	m_rb->SetPos(m_rb->GetNextPos());
}
