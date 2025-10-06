#include "CharacterStateBase.h"
#include "../../../General/Collision/Physics.h"

CharacterStateBase::CharacterStateBase(std::weak_ptr<Actor> owner):
	m_owner(owner)
{
}

CharacterStateBase::~CharacterStateBase()
{
}

void CharacterStateBase::ChangeState(std::shared_ptr<CharacterStateBase> nextState)
{
	//状態変化
	m_nextState = move(nextState);
}

Vector3 CharacterStateBase::GetNextNomVecFromRayCast(std::shared_ptr<Actor> coll, Vector3 targetPos, Vector3 moveDir, float speed, int beforeMoveToleranceNum, int afterMoveToleranceNum)
{
	//自分の位置
	Vector3 myPos = coll->GetPos();
	//移動ベクトル
	Vector3 moveVec = moveDir;
	if (moveVec.SqMagnitude() > 0.0f)
	{
		moveVec = moveVec.Normalize();
	}
	//レイを飛ばす
	auto& physics = Physics::GetInstance();
	//遮る物があるなら(無視する数より多いならtrue)
	auto amamam = physics.RayCast(myPos, targetPos).size();
	if (physics.RayCast(myPos, targetPos).size() > beforeMoveToleranceNum)
	{
		// 回避ベクトル候補（右・左）
		Vector3 right = moveVec.Cross(Vector3::Up());
		if (right.SqMagnitude() > 0.0f)right = right.Normalize();
		Vector3 left = Vector3::Up().Cross(moveVec);
		if (left.SqMagnitude() > 0.0f)left = left.Normalize();

		Vector3 rightPos = myPos + right * speed;
		Vector3 leftPos = myPos + left * speed;

		//右方向に回避できるか(無視する数以下ならtrue)
		if (physics.RayCast(rightPos, targetPos).size() <= afterMoveToleranceNum)
		{
			moveVec = right;
		}
		else
		{
			moveVec = left;
		}
	}
	return moveVec;
}
