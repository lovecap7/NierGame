#pragma once
#include "../../General/Collision/Rigidbody.h"
#include "../../General/Math/MyMath.h"
class Camera
{
public:
    Camera();
    ~Camera();
    void Init();
    void Update(const Vector3& targetPos);
    const Vector3& GetPos() const { return m_cameraPos; }
    const Vector3& GetLook() const { return m_look; }
	const Quaternion& GetRot() const { return m_rotH; }
private:
    //ƒJƒƒ‰‹——£
    float m_distance;

    //ã‰º§ŒÀ
    float m_vertexAngle = 0.0f;
    //Œü‚«
    Vector3 m_front = Vector3(0, 0, -1);
    Vector3 m_right = Vector3(1, 0, 0);
    Vector3 m_look;
    //‰ñ“]
    Quaternion m_rotH;
    //ˆÊ’u
    Vector3 m_targetPos;
    Vector3 m_cameraPos;
};

