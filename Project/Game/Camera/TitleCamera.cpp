#include "TitleCamera.h"

namespace
{
	//カメラ回転
	constexpr float kCameraRotateSpeed = 0.2f;
	//カメラ距離
	constexpr float kCameraDistance = 100.0f;
	//カメラ視界高さ
	constexpr float kCameraViewPosY = 30.0f;
	//カメラ高さ
	constexpr float kCameraPosY = 90.0f;
	//ニアファー
	constexpr float kNear = 0.1f;
	constexpr float kFar = 20000.0f;
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
	//値の初期化
	m_distance = kCameraDistance;
	m_vertexAngle = 0.0f;
	m_front = Vector3::Forward();
	m_right = Vector3::Right();
	m_look = m_front;
	m_rotH = Quaternion::IdentityQ();
	m_viewPos = Vector3::Zero();
	m_cameraPos = Vector3{ 0.0f,kCameraPosY,0.0f };
}

void TitleCamera::Update()
{
	//ニアファー
	SetCameraNearFar(kNear, kFar);

	m_front = Quaternion::AngleAxis(kCameraRotateSpeed * MyMath::DEG_2_RAD, Vector3::Up()) * m_front;
	if(m_front.SqMagnitude() > 0.0f)
	{
		m_front.Normalize();
	}
	m_viewPos = m_cameraPos + (m_front * m_distance);
	m_viewPos.y += kCameraViewPosY;

	//反映 
	DxLib::SetCameraPositionAndTarget_UpVecY(
		m_cameraPos.ToDxLibVector(),
		m_viewPos.ToDxLibVector()
	);
}

void TitleCamera::Draw() const
{
}
