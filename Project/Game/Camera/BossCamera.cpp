#include "BossCamera.h"
#include "CameraController.h"
#include "../Actor/ActorManager.h"

namespace
{
	//ニアファー
	constexpr float kNear = 10.0f;
	constexpr float kFar = 20000.0f;
	//Lerp
	constexpr float kCameraLerp = 0.05f;

	//フレーム
	constexpr int kPopFrame = 120;
}

BossCamera::BossCamera(Vector3 bossPos, Vector3 bossDir, float distance, std::weak_ptr<ActorManager> pActorManager, bool isStart) :
	CameraBase(),
	m_bossPos(bossPos),
	m_bossDir(bossDir),
	m_pActorManager(pActorManager),
	m_countFrame(0),
	m_isStart(isStart)
{
	m_distance = distance;
}

BossCamera::~BossCamera()
{
	//キャラクターを行動可能状態に
	if (m_pActorManager.expired())return;
	m_pActorManager.lock()->AllOperate();
}

void BossCamera::Init()
{
	//値の初期化
	m_countFrame = 0;
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

void BossCamera::Update()
{
	//ニアファー
	SetCameraNearFar(kNear, kFar);

	if (m_isStart)
	{
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
	else
	{

	}
}

void BossCamera::Draw() const
{

}

