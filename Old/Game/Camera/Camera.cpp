#include "Camera.h"
#include "../../General/Input.h"
#include "../../General/Collision/SphereCollider.h"

namespace
{
	//カメラ距離
	constexpr float kDistance = 1000.0f;
	//回転速度
    constexpr float kHorizontalSpeed = 0.01f;
    constexpr float kVerticalSpeed = -0.01f;
	//上下限界角度
    constexpr float kLimitAngle = 89.0f;
    //入力値角度変換
	constexpr float kInputToAngle = 0.1f;
	//カメラの高さ
	constexpr float kCameraHeight = 200.0f;
}

Camera::Camera():
    m_distance(0.0f),
    m_front(Vector3::Forward()),
    m_right(Vector3::Right()),
	m_look(m_front),
	m_rotH(Quaternion::IdentityQ())
{
}

Camera::~Camera() {}

void Camera::Init() 
{
    m_distance = kDistance;
    m_front = Vector3::Forward();
    m_right = Vector3::Right();
    m_look = m_front;
    m_rotH = Quaternion::IdentityQ();
}

void Camera::Update(const Vector3& targetPos)
{
    m_targetPos = targetPos;
	m_targetPos.y += kCameraHeight; //少し上を見る

	auto& input = Input::GetInstance();
	Vector2 stick(input.GetStickInfo().rightStickX * kInputToAngle, input.GetStickInfo().rightStickY * kInputToAngle);
    //入力による回転
    float rotH =  stick.x * kHorizontalSpeed;
    float rotV = stick.y * kVerticalSpeed;

    //左右回転（Up軸）
    auto qH = Quaternion::AngleAxis(rotH * MyMath::DEG_2_RAD, Vector3::Up());
    m_front = qH * m_front;
    m_right = qH * m_right;

    //上下回転（Right軸）
    m_vertexAngle = MathSub::ClampFloat((m_vertexAngle + rotV), -kLimitAngle, kLimitAngle);
    auto qV = Quaternion::AngleAxis(m_vertexAngle * MyMath::DEG_2_RAD, m_right);
    m_look = qV * m_front;
    if(m_look.SqMagnitude() > 0.0f)
    {
        m_look = m_look.Normalize();
    }
    //回転量の保存
	m_rotH = qH * m_rotH;

    //理想カメラ位置
    Vector3 idealPos;
    idealPos = m_targetPos - m_look * m_distance;
    m_cameraPos = idealPos;
    // DxLibに反映
    SetCameraPositionAndTarget_UpVecY(
        m_cameraPos.ToDxLibVector(),
        m_targetPos.ToDxLibVector()
    );
}
