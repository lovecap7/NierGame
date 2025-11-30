#include "StartCamera.h"
#include "CameraController.h"
#include "../Actor/Character/Player/Player.h"
#include "../../General/Model.h"

namespace
{
	//ニアファー
	constexpr float kNear = 10.0f;
	constexpr float kFar = 20000.0f;

	//角度
	constexpr float kFirstAngle = -70.0f;
	//距離
	constexpr float kFirstDistance = 60.0f;

	//第一フェーズ
	constexpr int kFirstPhaseFrame = 202;
	//横からのアングル
	constexpr float kSideAngle = 85.0f;
	//上昇速度
	constexpr float kHeightAdjustSpeed = 0.6f;
	//少し下を見る
	constexpr float kLookDownAdjust = -10.0f;
	

	//第二フェーズ
	constexpr int kSecondPhaseFrame = 260;
	//距離
	constexpr float kSecondDistance = 147.0f;
	//高さ
	constexpr float kSecondHeight = 95.0f;
	//Lerpの割合
	constexpr float kLerpRate = 0.5f;
	//少し上を見る
	constexpr float kLookUpAdjust = 10.0f;

}

StartCamera::StartCamera(std::weak_ptr<Player> pPlayer):
	CameraBase(),
	m_pPlayer(pPlayer),
	m_rotaAngle(kFirstAngle),
	m_heightAdjust(0.0f),
	m_countFrame(0)
{
	
}

StartCamera::~StartCamera()
{
}

void StartCamera::Init()
{
	m_distance = kFirstDistance;
	m_vertexAngle = 0.0f;
	m_front = Vector3::Forward();
	m_right = Vector3::Right();
	m_look = m_front;
	m_rotH = Quaternion::IdentityQ();
	m_viewPos = Vector3::Zero();
	m_cameraPos = Vector3::Zero();
	m_rotaAngle = kFirstAngle;
	m_heightAdjust = 0.0f;
	m_countFrame = 0;
	if (m_pPlayer.expired())return;
	auto player = m_pPlayer.lock();
	m_look = player->GetPos();
	m_cameraPos = player->GetPos() + player->GetModel()->GetDir() * m_distance;
}

void StartCamera::Update()
{
	//レンダーターゲットが毎フレーム更新されるので
	//ニアクリップとファークリップを毎フレーム設定し直す
	SetCameraNearFar(kNear, kFar);

	if (m_pPlayer.expired())return;
	auto player = m_pPlayer.lock();

	++m_countFrame;

	if (m_countFrame <= kFirstPhaseFrame)
	{
		++m_rotaAngle;
		if(m_rotaAngle > kSideAngle)
		{
			m_rotaAngle = kSideAngle;
		}
		else
		{
			m_heightAdjust += kHeightAdjustSpeed;
		}
		m_viewPos = player->GetPos();
		m_cameraPos = (Quaternion::AngleAxis(m_rotaAngle * MyMath::DEG_2_RAD, Vector3::Up()) * (player->GetModel()->GetDir() * m_distance)) + player->GetPos();
		m_cameraPos.y += m_heightAdjust;
		m_viewPos.y = m_cameraPos.y + kLookDownAdjust;
	}
	else if (m_countFrame <= kSecondPhaseFrame)
	{
		m_distance = MathSub::Lerp(m_distance, kSecondDistance, kLerpRate);
		m_heightAdjust = MathSub::Lerp(m_heightAdjust, kSecondHeight, kLerpRate);
		m_viewPos = player->GetPos();
		m_cameraPos = (Quaternion::AngleAxis(m_rotaAngle * MyMath::DEG_2_RAD, Vector3::Up()) * (player->GetModel()->GetDir() * m_distance)) + player->GetPos();
		m_cameraPos.y += m_heightAdjust;
		m_viewPos.y = m_cameraPos.y + kLookUpAdjust;
	}
	else
	{
		//反映 
		DxLib::SetCameraPositionAndTarget_UpVecY(
			m_cameraPos.ToDxLibVector(),
			m_viewPos.ToDxLibVector()
		);
		auto& cameraController = CameraController::GetInstance();
		cameraController.PopCamera(m_cameraPos, m_viewPos);
		return;
	}
	//反映 
	DxLib::SetCameraPositionAndTarget_UpVecY(
		m_cameraPos.ToDxLibVector(),
		m_viewPos.ToDxLibVector()
	);
	
}
