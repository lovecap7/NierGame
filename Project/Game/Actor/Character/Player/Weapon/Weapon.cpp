#include "Weapon.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/Collision/SphereCollider.h"

namespace
{
	constexpr float kIdleOffsetY = 30.0f;
}

Weapon::Weapon(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData, Shape::Sphere, pActorManager),
	m_ownerHandle(-1),
	m_battleSlotIndex(0),
	m_idleSlotIndex(0),
	m_isBattle(false)
{
	m_model->SetRotSpeed(1);
}

Weapon::~Weapon()
{
}

void Weapon::Init()
{
	//Physicsに登録
	Collidable::Init();
}
void Weapon::Update()
{
}

void Weapon::Draw() const
{
	m_model->Draw();
}

void Weapon::End()
{
	//Physicsから解除
	Collidable::End();
	//モデルの解放
	m_model->End();
}

void Weapon::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Weapon::Complete()
{
	//戦闘中なら
	if (m_isBattle)
	{
		//マトリクスから向きの計算
		Matrix4x4 mat = Matrix4x4::ToMatrix4x4(MV1GetFrameLocalWorldMatrix(m_ownerHandle, m_battleSlotIndex));
		//大きさ
		mat = mat * Matrix4x4::ScaleMatrix4x4(m_actorData->m_scale);
		//座標
		Vector3 weaponPos = MV1GetFramePosition(m_ownerHandle, m_battleSlotIndex);
		mat.mat[3][0] = weaponPos.x;
		mat.mat[3][1] = weaponPos.y;
		mat.mat[3][2] = weaponPos.z;
		//セット
		SetWeaponMat(mat);
		//モデルの位置を更新
		m_model->SetPos(weaponPos.ToDxLibVector());
		//Rigidbodyの位置を更新
		m_rb->m_pos = m_rb->GetNextPos();
	}
	//戦闘中ではないなら
	else
	{
		//マトリクスから向きの計算
		Matrix4x4 mat = Matrix4x4::ToMatrix4x4(MV1GetFrameLocalWorldMatrix(m_ownerHandle, m_idleSlotIndex));
		//大きさ
		mat = mat * Matrix4x4::ScaleMatrix4x4(m_actorData->m_scale);
		//座標
		Vector3 weaponPos = MV1GetFramePosition(m_ownerHandle, m_idleSlotIndex);
		mat.mat[3][0] = weaponPos.x;
		mat.mat[3][1] = weaponPos.y + kIdleOffsetY;
		mat.mat[3][2] = weaponPos.z;
		//セット
		SetWeaponMat(mat);
		//モデルの位置を更新
		m_model->SetPos(weaponPos.ToDxLibVector());
		//Rigidbodyの位置を更新
		m_rb->m_pos = m_rb->GetNextPos();
	}
}

void Weapon::SetWeaponMat(Matrix4x4 mat)
{
	m_model->SetMatrix(mat);
}

