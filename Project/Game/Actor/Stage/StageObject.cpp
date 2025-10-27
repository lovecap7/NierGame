#include "StageObject.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Model.h"
StageObject::StageObject(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager):
	Actor(actorData,Shape::Polygon,pActorManager)
{
}

StageObject::~StageObject()
{
}

void StageObject::Init()
{
	if (m_isThrough)return;
	Collidable::Init();
	MV1SetupCollInfo(m_model->GetModelHandle(), -1);
}

void StageObject::Update()
{
	m_model->Update();
	m_model->ApplyMat();
	//“–‚½‚è”»’è‚ð‚·‚é
	if (!m_isThrough)
	{
		MV1RefreshCollInfo(m_model->GetModelHandle(), -1);
	}
}

void StageObject::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void StageObject::Draw() const
{
	//“–‚½‚è”»’è‚ð‚·‚é‚È‚ç•`‰æ‚·‚é
	if (m_isThrough)return;
	m_model->Draw();
}

void StageObject::Complete()
{
}

void StageObject::End()
{
	m_model->End();
	if (m_isThrough)return;
	Collidable::End();
}
