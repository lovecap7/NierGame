#include "PlayerCamera.h"
#include "../../General/Input.h"
#include "../../General/Collision/SphereCollider.h"
#include "../Actor/Debug/DebugPlayer.h"
#include "../../Main/Application.h"

namespace
{
    //カメラ距離
    constexpr float kDistance = 500.0f;
    //回転速度
    constexpr float kHorizontalSpeed = 0.02f;
    constexpr float kVerticalSpeed = 0.02f;
    //上下限界角度
    constexpr float kLimitAngle = 80.0f;
    //入力値角度変換
    constexpr float kInputToAngle = 0.1f;
    //カメラの高さ
    constexpr float kCameraHeight = 300.0f;
}

PlayerCamera::PlayerCamera() :
	CameraBase(),
	m_isLockOn(false),
	m_lockOnTarget(),
	m_playerPos(Vector3::Zero())
{
}

PlayerCamera::~PlayerCamera() {}

void PlayerCamera::Init()
{
    m_distance = kDistance;
	m_vertexAngle = 0.0f;
    m_front = Vector3::Forward();
    m_right = Vector3::Right();
    m_look = m_front;
    m_rotH = Quaternion::IdentityQ();
	m_viewPos = Vector3::Zero();
	m_cameraPos = Vector3::Zero();
	m_isLockOn = false;
	m_playerPos = Vector3::Zero();
}

void PlayerCamera::Update()
{
    Vector3 playerPos = m_playerPos;
    Vector3 targetPos = playerPos;
    //ターゲット
    targetPos.y += kCameraHeight; //少し上を見る
    auto& input = Input::GetInstance();
    Vector2 stick(input.GetStickInfo().rightStickX * kInputToAngle, input.GetStickInfo().rightStickY * kInputToAngle);
    //入力による回転
    float rotH = stick.x * kHorizontalSpeed;
    float rotV = stick.y * kVerticalSpeed;

    //左右回転（Up軸）
    auto qH = Quaternion::AngleAxis(rotH * MyMath::DEG_2_RAD, Vector3::Up());
    m_front = qH * m_front;
    m_right = qH * m_right;

    //上下回転（Right軸）
    m_vertexAngle = MathSub::ClampFloat((m_vertexAngle + rotV), -kLimitAngle, kLimitAngle);
    auto qV = Quaternion::AngleAxis(m_vertexAngle * MyMath::DEG_2_RAD, m_right);
    m_look = qV * m_front;
    if (m_look.SqMagnitude() > 0.0f)
    {
        m_look = m_look.Normalize();
    }
    //回転量の保存
    m_rotH = qH * m_rotH;

    //理想カメラ位置
    Vector3 nextPos;
    nextPos = targetPos - m_look * m_distance;
    //ロックオン中ならターゲットをロックオン対象にする
    Vector3 viewPos = targetPos;
    if (m_isLockOn && !m_lockOnTarget.expired())
    {
        //ターゲットとロックオン対象の中間地点を見る
        viewPos = Vector3::Lerp(m_lockOnTarget.lock()->GetNextPos(), playerPos, 0.4f);
    }

    //位置確定
    m_cameraPos = Vector3::Lerp(m_cameraPos, nextPos, 0.05f);
   
	m_viewPos = Vector3::Lerp(m_viewPos, viewPos, 0.05f);
    // DxLibに反映
    SetCameraPositionAndTarget_UpVecY(
        m_cameraPos.ToDxLibVector(),
        m_viewPos.ToDxLibVector()
    );
}

void PlayerCamera::StartLockOn(std::weak_ptr<Actor> lockOnTarget)
{
	if (lockOnTarget.expired())return;
	m_isLockOn = true;
	m_lockOnTarget = lockOnTarget;
}

void PlayerCamera::EndLockOn()
{
	m_isLockOn = false;
	m_lockOnTarget.reset();
}