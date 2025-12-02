#include "TitleCamera.h"

namespace
{
	//ƒJƒƒ‰‰ñ“]
	constexpr float kCameraRotateSpeed = 0.5f;
}

TitleCamera::TitleCamera():
	CameraBase()
{
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Init()
{
	//’l‚Ì‰Šú‰»
	m_distance = 100.0f;
	m_vertexAngle = 0.0f;
	m_front = Vector3::Forward();
	m_right = Vector3::Right();
	m_look = m_front;
	m_rotH = Quaternion::IdentityQ();
	m_viewPos = Vector3::Zero();
	m_cameraPos = Vector3::Zero();
}

void TitleCamera::Update()
{
	m_front = Quaternion::AngleAxis(kCameraRotateSpeed * MyMath::DEG_2_RAD, Vector3::Up()) * m_front;
	if(m_front.SqMagnitude() > 0.0f)
	{
		m_front.Normalize();
	}
	m_viewPos = m_front * m_distance;

	//”½‰f 
	DxLib::SetCameraPositionAndTarget_UpVecY(
		m_cameraPos.ToDxLibVector(),
		m_viewPos.ToDxLibVector()
	);
}

void TitleCamera::Draw() const
{
}
