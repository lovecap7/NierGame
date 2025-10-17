#include "PodStateBase.h"
#include "Pod.h"

PodStateBase::PodStateBase(std::weak_ptr<Actor> pod):
	CharacterStateBase(pod)
{
}

PodStateBase::~PodStateBase()
{
}

Vector3 PodStateBase::GetPodPos(Vector3 targetPos, Vector3 cameraDir, float upH, float right, float back)
{
	//ターゲットの近くに移動
	targetPos.y += upH;
	//カメラに対して右
	Vector3 rightDir = cameraDir.Cross(Vector3::Up());
	if (rightDir.SqMagnitude() > 0.0f)
	{
		rightDir = rightDir.Normalize();
	}
	targetPos += rightDir * right + cameraDir * back;
	//座標
	Vector3 nextPos = targetPos;
	return nextPos;
}
