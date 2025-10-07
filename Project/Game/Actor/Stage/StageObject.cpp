#include "StageObject.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Model.h"
StageObject::StageObject(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager):
	Actor(actorData,Shape::Polygon,pActorManager)
{
	/*if (m_actorData->m_modelPath == L"Collision/Cube")
	{
		Vector3 scale = m_model->GetScale() * 0.01f;
		m_model->SetScale(scale.ToDxLibVector());
	}*/
}

StageObject::~StageObject()
{
}

void StageObject::Init()
{
	Collidable::Init();
	if (m_isThrough)return;
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
	//“–‚½‚è”»’è‚ð‚·‚é‚È‚ç•`‰æ‚µ‚È‚¢
	//if (!m_isThrough)return;
	m_model->Draw();
}

void StageObject::Complete()
{
}

void StageObject::End()
{
	Collidable::End();
	m_model->End();
}
