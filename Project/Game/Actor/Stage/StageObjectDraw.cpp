#include "StageObjectDraw.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Model.h"

StageObjectDraw::StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle):
	Actor(Shape::None)
{
	//ƒ‚ƒfƒ‹
	m_model = std::make_shared<Model>(modelHandle, pos);
	m_model->SetScale(scale);
	m_model->SetRot(angle);
}

StageObjectDraw::~StageObjectDraw()
{
}

void StageObjectDraw::Update()
{
	m_model->Update();
}

void StageObjectDraw::Draw() const
{
	m_model->Draw();
}

void StageObjectDraw::End()
{
	m_model->End();
}
