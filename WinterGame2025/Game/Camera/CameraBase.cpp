#include "CameraBase.h"

CameraBase::CameraBase():
    m_distance(0.0f),
	m_vertexAngle(0.0f),
    m_front(Vector3::Forward()),
    m_right(Vector3::Right()),
    m_look(m_front),
    m_rotH(Quaternion::IdentityQ()),
	m_targetPos(Vector3::Zero()),
	m_cameraPos(Vector3::Zero())
{
}

CameraBase::~CameraBase()
{
}
