#include "BossStartCamera.h"
#include "CameraController.h"
#include "../Actor/ActorManager.h"

namespace
{
	//カメラ距離
	constexpr float kCameraDistance = 800.0f;
	//ニアファー
	constexpr float kNear = 10.0f;
	constexpr float kFar = 20000.0f;
	//Lerp
	constexpr float kCameraLerp = 0.05f;

	//フレーム
	constexpr int kPopFrame = 120;
}

BossStartCamera::BossStartCamera(Vector3 bossPos, Vector3 bossDir, std::weak_ptr<ActorManager> pActorManager):
	CameraBase(),
	m_bossPos(bossPos),
	m_bossDir(bossDir),
	m_pActorManager(pActorManager),
	m_countFrame(0)
{
	
}

BossStartCamera::~BossStartCamera()
{
	//キャラクターを行動可能状態に
	if (m_pActorManager.expired())return;
	m_pActorManager.lock()->AllOperate();
}

void BossStartCamera::Init()
{
	//値の初期化
	m_countFrame = 0;
	m_distance = kCameraDistance;
	m_vertexAngle = 0.0f;
	m_front = Vector3::Forward();
	m_right = Vector3::Right();
	if (m_bossDir.SqMagnitude() > 0.0f)
	{
		m_bossDir = m_bossDir.Normalize();
	}
	m_look = m_bossDir * -1;
	m_rotH = Quaternion::IdentityQ();
	m_viewPos = m_bossPos;
	m_cameraPos = CameraController::GetInstance().GetBaseCameraPos();

	//キャラクターを待機状態に
	if (m_pActorManager.expired())return;
	m_pActorManager.lock()->AllWait();
}

void BossStartCamera::Update()
{
	//ニアファー
	SetCameraNearFar(kNear, kFar);

	Vector3 nextPos = m_bossPos + m_bossDir * m_distance;
	m_cameraPos = Vector3::Lerp(m_cameraPos, nextPos, kCameraLerp);

	//反映 
	DxLib::SetCameraPositionAndTarget_UpVecY(
		m_cameraPos.ToDxLibVector(),
		m_viewPos.ToDxLibVector()
	);

	if (m_countFrame >= kPopFrame)
	{
		CameraController::GetInstance().PopCamera();
		return;
	}
	++m_countFrame;

}

void BossStartCamera::Draw() const
{

}
