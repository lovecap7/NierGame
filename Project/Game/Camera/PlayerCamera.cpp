#include "PlayerCamera.h"
#include "../../General/Input.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/Physics.h"
#include "../../Main/Application.h"
#include "../Actor/DebugActor/DebugPlayer.h"
#include "../Actor/ActorManager.h"
#include "../../General/Game.h"

namespace
{
    //カメラ設定定数
    constexpr float kDistance = 400.0f;         //カメラ距離
    constexpr float kHorizontalSpeed = 0.05f;   //水平回転速度
    constexpr float kVerticalSpeed = 0.02f;     //垂直回転速度
    constexpr float kLimitAngle = 80.0f;        //上下限界角度
    constexpr float kInputToAngle = 0.1f;       //スティック入力→角度変換係数
    constexpr float kCameraHeight = 150.0f;     //プレイヤーからのカメラ高さ
    //操作してないときの旋回速度
    constexpr float kReturnSpeed = 0.02f; // 戻るスピード

	//ロックオン時の設定
    //カメラオフセット（右に寄せる）
    constexpr float kRightOffset = 150.0f;   // 右オフセット距離
    constexpr float kBackOffset = 350.0f;   // 後方距離
    constexpr float kUpOffset = 200.0f;   // 上方向オフセット
    constexpr float kFollowSpeed = 0.15f;
    constexpr float kRotFollowSpeed = 0.2f; // 追従回転速度
	//右向きを決める内積
	constexpr float kRightDot = 0.7f;
    //ロックオン中のカメラ操作時の視点
	constexpr float kLockOnViewInput = 100.0f;
}

PlayerCamera::PlayerCamera() :
	CameraBase(),
	m_isLockOn(false),
	m_lockOnTarget(),
	m_playerPos(Vector3::Zero()),
    m_lockOnSide(kRightOffset),
    m_nextlockOnSide(kRightOffset)
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
    m_lockOnSide = kRightOffset;
    m_nextlockOnSide = kRightOffset;
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
    if(islockOn)
    {
		//ロックオン更新
		LockOnUpdate(input, targetPos);
	}
    else
    {
		//通常更新
        NormalUpdate(input, targetPos);
    }
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

void PlayerCamera::NormalUpdate(Input& input, Vector3& targetPos)
{
    //入力が入っているとき
    if (input.GetStickInfo().IsRightStickInput())
    {
		//スティック入力でカメラ回転
        UpdateStickAngle(input);
    }
    else
    {
        //プレイヤーが移動を開始したらだんだんプレイヤーの向きにカメラをうごかす
        if (m_playerVec.SqMagnitude() > 0.0f)
        {
            Vector3 playerForward = m_playerDir;
            playerForward.y = 0.0f; // 水平面に限定
            if (playerForward.SqMagnitude() > 0.0f)
            {
                playerForward = playerForward.Normalize();
                //前方向をもとに他の方向の更新
                UpdateDirection(Vector3::Lerp(m_front, playerForward, kReturnSpeed));
            }
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
    Vector3 viewPos = targetPos;
    m_viewPos = viewPos;
    // DxLibに反映
    SetCameraPositionAndTarget_UpVecY(
        m_cameraPos.ToDxLibVector(),
        m_viewPos.ToDxLibVector()
    );
}

void PlayerCamera::LockOnUpdate(Input& input, Vector3& targetPos)
{
    if (m_lockOnTarget.expired()) return;
    auto lockOnTarget = m_lockOnTarget.lock();

    //プレイヤーとターゲットの位置
    Vector3 playerPos = m_playerPos;
    Vector3 enemyPos = lockOnTarget->GetNextPos();

    //プレイヤーから敵方向を求める
    Vector3 toEnemy = (enemyPos - playerPos);
    if (toEnemy.SqMagnitude() > 0.0f)
    {
        toEnemy = toEnemy.Normalize();
    }

    //中点（注視点）を計算
    Vector3 center = (playerPos + enemyPos) * 0.5f;
    center.y += kCameraHeight; // 少し上を見る
    targetPos = center;

    // カメラの理想位置
    Vector3 basePos = playerPos;


    //入力が入っているとき
	bool isStickInput = input.GetStickInfo().IsRightStickInput();
    if (isStickInput)
    {
        //スティック入力でカメラ回転
        UpdateStickAngle(input);

		//理想位置計算
		basePos.y += kCameraHeight; // 少し上を見る
        basePos += m_look * -kBackOffset;

		//視点はプレイヤーからターゲットへのベクトルの少し前方
		targetPos = playerPos + toEnemy * kLockOnViewInput;
		targetPos.y += kCameraHeight;
    }
    else
    {
        //水平方向リセット
		m_vertexAngle = 0.0f;

        //プレイヤー右方向を求める
        Vector3 playerRight = Vector3::Up().Cross(toEnemy);
        if (playerRight.SqMagnitude() > 0.0f)
        {
            playerRight = playerRight.Normalize();
        }

        //プレイヤーが右を向いているなら左に動く
        if (m_playerDir.Dot(playerRight) > kRightDot)
        {
            m_nextlockOnSide = -kRightOffset;
        }
        //プレイヤーが左を向いているなら右に動く
        else if (m_playerDir.Dot(playerRight) < -kRightDot)
        {
            m_nextlockOnSide = kRightOffset;
        }
        m_lockOnSide = MathSub::Lerp(m_lockOnSide, m_nextlockOnSide, 0.05f);

        basePos -= (toEnemy * kBackOffset);
        basePos += (playerRight * m_lockOnSide);                // 横にオフセット
        basePos += Vector3::Up() * kUpOffset;                   //上にオフセット
    }
    Vector3 idealCamPos = basePos;
    //衝突補正（壁など）
    Vector3 nextPos = Physics::GetInstance().GetCameraRatCastNearEndPos(targetPos, idealCamPos);

    //補間してなめらかに追従
    m_cameraPos = Vector3::Lerp(m_cameraPos, nextPos, kFollowSpeed);

    //視点を更新
    m_viewPos = Vector3::Lerp(m_viewPos, targetPos, kFollowSpeed);

    if (!isStickInput)
    {
        //前方向の更新
        Vector3 frontDir = (m_viewPos - m_cameraPos);
        frontDir.y = 0.0f;  // 水平面に限定
        if (frontDir.SqMagnitude() > 0.0f)
        {
            frontDir = frontDir.Normalize();
        }
        //前方向をもとに他の方向の更新
        UpdateDirection(Vector3::Lerp(m_front, frontDir, kRotFollowSpeed));
    }

    //反映 
    DxLib::SetCameraPositionAndTarget_UpVecY(
        m_cameraPos.ToDxLibVector(),
        m_viewPos.ToDxLibVector()
    );
}

void PlayerCamera::UpdateDirection(Vector3 newFront)
{
	m_front = newFront;
    if(m_front.SqMagnitude() > 0.0f)
    {
		m_front = m_front.Normalize();
	}

	//右方向更新
    m_right = Vector3::Up().Cross(m_front);
    if (m_right.SqMagnitude() > 0.0f)
    {
        m_right = m_right.Normalize();
	}

    // 縦角反映
    m_look = Quaternion::AngleAxis(m_vertexAngle * MyMath::DEG_2_RAD, m_right) * m_front;
    if (m_look.SqMagnitude() > 0.0f)
    {
        m_look = m_look.Normalize();
    }

    // クォータニオン更新
    m_rotH = Quaternion::CalcHorizontalQuat(m_front);
}

void PlayerCamera::UpdateStickAngle(Input& input)
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
