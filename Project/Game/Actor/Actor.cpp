#include "Actor.h"
#include "../../General/Collision/Rigidbody.h"
#include "../../General/Collision/Physics.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/PolygonCollider.h"
#include "../../General/Collision/TorusCollider.h"
#include "../../General/Model.h"
#include "../../General/AssetManager.h"

Actor::Actor(std::shared_ptr<ActorData> actorData,Shape shape, std::weak_ptr<ActorManager> pActorManager):
	Collidable(shape),
	m_pActorManager(pActorManager),
	m_actorData(actorData),
	m_isDelete(false),
	m_id(0),
	m_isSetId(false)
{
	if (!m_actorData)return;
	//モデル
	auto& assetManager = AssetManager::GetInstance();
	int handle = assetManager.GetModelHandle(m_actorData->m_modelPath);
	m_model = std::make_shared<Model>(handle, m_actorData->m_pos.ToDxLibVector());
	//大きさ
	m_model->SetScale(m_actorData->m_scale.ToDxLibVector());
	//回転量
	m_model->SetRot(m_actorData->m_rot.ToDxLibVector());
	//座標
	m_rb->SetPos(m_actorData->m_pos);
	//コライダーの設定
	switch (shape)
	{
	case Shape::Sphere:
	{
		auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
		coll->SetRadius(m_actorData->m_collRadius);
		break;
	}
	case Shape::Capsule:
	{
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		coll->SetRadius(m_actorData->m_collRadius);
		Vector3 endPos = m_rb->m_pos;
		endPos.y += m_actorData->m_collHeight;
		coll->SetEndPos(endPos);
		break;
	}
	case Shape::Polygon:
	{
		auto coll = std::dynamic_pointer_cast<PolygonCollider>(m_collisionData);
		coll->SetModelHandle(handle);
		break;
	}
	case Shape::Torus:
	{
		auto coll = std::dynamic_pointer_cast<TorusCollider>(m_collisionData);
		coll->SetRadius(m_actorData->m_collRadius);
		coll->SetRange(m_actorData->m_collHeight);
		break;
	}
	default:
		break;
	};
	AllSetting(m_actorData->m_priority, m_actorData->m_gameTag, m_actorData->m_isTrough, m_actorData->m_isTrigger, m_actorData->m_isGravity);
}

void Actor::SetID(int id)
{
	//すでに登録してるなら
	if (m_isSetId)return;
	m_id = id;
	m_isSetId = true;
}

Vector3 Actor::GetPos() const
{
	return m_rb->m_pos;
}

Vector3 Actor::GetNextPos() const
{
	return m_rb->GetNextPos();
}

void Actor::SetTimeScale(float scale)
{
	Collidable::SetTimeScale(scale);
	//モデルにも
	if (m_model)
	{
		m_model->SetTimeScale(scale);
	}
}
