#include "PlayerCamera.h"
#include "../../General/Input.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/Physics.h"
#include "../../Main/Application.h"
#include "../Actor/DebugActor/DebugPlayer.h"
#include "../../General/Collision/Physics.h"

namespace
{
    //カメラ設定定数
    constexpr float kDistance = 400.0f;         //カメラ距離
    constexpr float kHorizontalSpeed = 0.05f;   //水平回転速度
    constexpr float kVerticalSpeed = 0.02f;     //垂直回転速度
    constexpr float kLimitAngle = 80.0f;        //上下限界角度
    constexpr float kInputToAngle = 0.1f;       //スティック入力→角度変換係数
    constexpr float kCameraHeight = 150.0f;     //プレイヤーからのカメラ高さ

    //追従・補間設定定数
    constexpr float kNormalFollowLerp = 0.005f;  //通常時の追従速度
    constexpr float kForwardFollowLerp = 0.2f;  //プレイヤーが前進中の追従速度
    constexpr float kLerpBlendSpeed = 0.001f;     //lerp率の補間速度
    constexpr float kDotThreshold = 0.7f;       //前進中」と判定するためのDot閾値
    constexpr float kLockOnFollowSpeed = 0.1f;  //ロックオン時の回転追従速度
}

PlayerCamera::PlayerCamera() :
	CameraBase(),
	m_isLockOn(false),
	m_lockOnTarget(),
	m_playerPos(Vector3::Zero()),
    m_lerpRate(0.0f)
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
    m_lerpRate = kNormalFollowLerp;
}

void PlayerCamera::Update()
{
    //プレイヤー
    Vector3 playerPos = m_playerPos;
    //ターゲット
    Vector3 targetPos = playerPos;
    targetPos.y += kCameraHeight; //少し上を見る
    //ロックオン状態
    bool islockOn = m_isLockOn && !m_lockOnTarget.expired();

    auto& input = Input::GetInstance();
    //入力が入っているとき
    if (input.GetStickInfo().IsRightStickInput())
    {
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
    }
    else
    {
        if (islockOn)
        {
            auto lockOn = m_lockOnTarget.lock();
            //ターゲットとプレイヤーの間
            Vector3 lockOnPos = Vector3::Lerp(lockOn->GetPos(), playerPos, 0.5f);
            Vector3 dir = lockOnPos - m_cameraPos;
            if (dir.SqMagnitude() > 0.0f)
            {
                dir = dir.Normalize();
            }
            Vector3 flatDir = dir;
            flatDir.y = 0.0f;
            UpdateCameraDirection(flatDir, kLockOnFollowSpeed); //ロックオンは即座に反映
        }
    }
    //理想カメラ位置
    Vector3 nextPos;
    nextPos = targetPos - m_look * m_distance;
    //位置補正
    nextPos = Physics::GetInstance().GetCameraRatCastNearEndPos(targetPos, nextPos);
    //位置確定
    m_cameraPos = nextPos;
    //視点確定
    //ロックオン中ならターゲットをロックオン対象にする
    Vector3 viewPos = targetPos;
    m_viewPos = viewPos;
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

void PlayerCamera::UpdateCameraDirection(const Vector3& targetDir, float followSpeed)
{
    Vector3 normTargetDir = targetDir;
    if (normTargetDir.SqMagnitude() > 0.0f)
    {
        normTargetDir = normTargetDir.Normalize();
    }

    // targetDirに向かってfrontを補間
    m_front = Vector3::Lerp(m_front, normTargetDir, followSpeed);
    if (m_front.SqMagnitude() > 0.0f)
    {
        m_front = m_front.Normalize();
    }
    // 向きベクトル更新
    m_right = Vector3::Up().Cross(m_front);
    if (m_right.SqMagnitude() > 0.0f)
    {
        m_right = m_right.Normalize();
    }
    m_look = Quaternion::AngleAxis(m_vertexAngle * MyMath::DEG_2_RAD, m_right) * m_front;
    if (m_look.SqMagnitude() > 0.0f)
    {
        m_look = m_look.Normalize();
    }

    // 水平方向のクォータニオンを保存
    m_rotH = Quaternion::CalcHorizontalQuat(m_front);
}