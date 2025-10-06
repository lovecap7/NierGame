#include "Sky.h"
#include <DxLib.h>
#include "../../../General/Model.h"
#include "../../Camera/GameCamera/GameCamera.h"

namespace
{
	constexpr float kScale = 2.5f;
	constexpr float kRotaAngle = 0.01f;
}


Sky::Sky(int handle) :
	Actor(Shape::None)
{
	//ƒ‚ƒfƒ‹
	m_model = std::make_shared<Model>(handle, Vector3::Zero().ToDxLibVector());
	m_model->SetScale(VGet(kScale, kScale, kScale));
}

Sky::~Sky()
{
}

void Sky::Init()
{
}

void Sky::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	m_model->SetPos(camera.lock()->GetPos().ToDxLibVector());
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	m_model->Update();
}

void Sky::Update()
{
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	m_model->Update();
}

void Sky::Draw() const
{
	m_model->Draw();
}

void Sky::End()
{
	m_model->End();
}
