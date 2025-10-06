#include "DebugWeapon.h"
#include "../../../General/Model.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"

DebugWeapon::DebugWeapon(int weaponHandle):
	Actor(Shape::Sphere),
	m_ownerHandle(-1),
	m_itemSlotIndex(0),
	m_scale(1.0f)
{
	//モデルのロード
	m_model = std::make_shared<Model>(weaponHandle, m_rb->m_pos.ToDxLibVector());
	m_model->SetRotSpeed(1);
}

DebugWeapon::~DebugWeapon()
{
}

void DebugWeapon::Init()
{
	//Physicsに登録
	Collidable::Init();
	//初期化
	m_rb->m_pos = Vector3::Zero();
	m_rb->m_vec = Vector3::Zero();
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Attack, false, false, false);
	//コライダーの設定
	auto sphereCollider = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	sphereCollider->SetRadius(20.0f);
}
void DebugWeapon::Update()
{
}

void DebugWeapon::Draw() const
{
	m_model->Draw();
}

void DebugWeapon::End()
{
	//Physicsから解除
	Collidable::End();
	//モデルの解放
	m_model->End();
}

void DebugWeapon::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void DebugWeapon::Complete()
{
	//マトリクスから向きの計算
	Matrix4x4 mat = Matrix4x4::ToMatrix4x4(MV1GetFrameLocalWorldMatrix(m_ownerHandle, m_itemSlotIndex));
	//大きさ
	mat = mat * Matrix4x4::ScaleMatrix4x4(Vector3(m_scale, m_scale, m_scale));
	//座標
	Vector3 weaponPos = MV1GetFramePosition(m_ownerHandle, m_itemSlotIndex);
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

void DebugWeapon::SetWeaponMat(Matrix4x4 mat)
{
	m_model->SetMatrix(mat);
}

