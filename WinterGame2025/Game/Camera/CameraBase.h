#pragma once
#include "../../General/Collision/Rigidbody.h"
#include "../../General/Math/MyMath.h"
class CameraBase abstract
{
public:
	CameraBase();
	virtual ~CameraBase();
	virtual void Init() abstract;
	virtual void Update() abstract;
	const Vector3& GetPos() const { return m_cameraPos; }
	const Vector3& GetLook() const { return m_look; }
	const Quaternion& GetRot() const { return m_rotH; }
protected:
    //カメラ距離
    float m_distance;
    //上下制限
    float m_vertexAngle;
    //向き
	Vector3 m_front;	 //前方向
	Vector3 m_right;	 //右方向
	Vector3 m_look;		 //注視方向
    //回転
    Quaternion m_rotH;
    //位置
	Vector3 m_targetPos; //注視点
	Vector3 m_cameraPos; //カメラ位置
};

