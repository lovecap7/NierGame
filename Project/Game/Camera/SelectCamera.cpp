#include "SelectCamera.h"

namespace
{
    //ãóó£
    constexpr float kDistance = 100.0f;
    //Lerp
    constexpr float kViewLerp = 0.1f;

    //ÉJÉÅÉâÇÃéãñÏäp
    constexpr float kPullPerspective = 60.0f * MyMath::DEG_2_RAD;
    constexpr float kZoomInPerspective = 30.0f * MyMath::DEG_2_RAD;
}

SelectCamera::SelectCamera():
	CameraBase(),
    m_nextViewPos(),
    m_perspective(kPullPerspective),
    m_nextPerspective(kPullPerspective)
{
}

SelectCamera::~SelectCamera()
{
}

void SelectCamera::Init()
{
    m_distance = kDistance;
    m_vertexAngle = 0.0f;
    m_front = Vector3::Forward();
    m_right = Vector3::Right();
    m_look = m_front;
    m_rotH = Quaternion::IdentityQ();
    m_cameraPos = Vector3::Zero();
    m_viewPos = m_cameraPos + (m_front * m_distance);
    m_nextViewPos = m_viewPos;
    m_perspective = kPullPerspective;
    m_nextPerspective = m_perspective;
}

void SelectCamera::Update()
{
    //éãì_
    m_nextViewPos = Vector3::Lerp(m_nextViewPos, m_viewPos, kViewLerp);

    //éãñÏäp
    m_perspective = MathSub::Lerp(m_perspective, m_nextPerspective, kViewLerp);

    // DxLibÇ…îΩâf
    DxLib::SetCameraPositionAndTarget_UpVecY(
        m_cameraPos.ToDxLibVector(),
        m_nextViewPos.ToDxLibVector()
    );
    //éãñÏäp
    SetupCamera_Perspective(m_perspective);
}

void SelectCamera::Draw() const
{
}

void SelectCamera::PullCamera()
{
    m_nextPerspective = kPullPerspective;
}

void SelectCamera::ZoomInCamera()
{
    m_nextPerspective = kZoomInPerspective;
}
