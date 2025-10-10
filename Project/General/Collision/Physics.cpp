#include "Physics.h"
#include "CollisionChecker.h"
#include "FixNextPosition.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "Rigidbody.h"
#include "../../Main/Application.h"
#include <cassert>

namespace
{
	//確認回数
	constexpr int kTryNum = 30;
	//重力
	const Vector3 kGroundGravity = { 0.0f, -5.0f, 0.0f };
	const Vector3 kAirGravity = { 0.0f, -0.5f, 0.0f };
}

void Physics::Init()
{
	m_isUpdate = true;
	m_collChecker = std::make_shared<CollisionChecker>();
	m_collProcessor = std::make_shared<FixNextPosition>();
}

void Physics::Entry(std::shared_ptr<Collidable> collidable)
{
	//すでに登録されているならしない
	auto it = std::find(m_collidables.begin(), m_collidables.end(), collidable);
	if (it != m_collidables.end())return;
	//コライダーの追加
	m_collidables.emplace_back(collidable);
}

void Physics::Exit(std::shared_ptr<Collidable> collidable)
{
	//登録されていないならしない
	auto it = std::find(m_collidables.begin(), m_collidables.end(), collidable);
	if (it == m_collidables.end())return;
	m_collidables.erase(it);
}

void Physics::Update()
{
	//更新をしないなら
	if (!m_isUpdate)return;
	//タイムスケールの初期化
	InitTimeScale();
	//重力
	Gravity();
	//床と壁のとの当たったフラグを初期化
	for (auto& collidable : m_collidables)
	{
		collidable->ResetHitFlag();
	}
	//遅延処理用
	std::list<OnCollideInfo> onCollideInfo;
	//一度も当たっていないのならループを終了する
	bool isOneMore = false;
	//補正したことで別のオブジェクトに当たる可能性があるので一定回数チャックする
	for (int i = 0;i < kTryNum;++i)
	{
		isOneMore = false;
		//当たり判定をチェック
		for (auto& collA : m_collidables)
		{
			//当たり判定を行わないなら飛ばす
			if (collA->GetGameTag() == GameTag::None)continue;
			if (collA->m_isThrough)continue;
			for (auto& collB : m_collidables)
			{
				//自分とは当たり判定をしない
				if (collA == collB)continue;
				//当たり判定を行わないなら飛ばす
				if (collB->GetGameTag() == GameTag::None)continue;
				if (collB->m_isThrough)continue;
				//当たってるなら
				if (m_collChecker->IsCollide(collA, collB))
				{
					//どちらもトリガーなではないなら
					if (!collA->m_isTrigger && !collB->m_isTrigger)
					{
						//衝突処理
						m_collProcessor->FixNextPos(collA, collB);
						//変更後の位置で再度ほかのコライダブルに当たる可能性があるので
						//もう一度チェックする必要がある
						isOneMore = true;
					}
					//これまでにこの組み合わせで当たった情報があるかをチェック
					bool isCollInfo = false;
					for (const auto& item : onCollideInfo)
					{
						// 既に通知リストに含まれていたら呼ばない
						if (item.owner == collA && item.colider == collB ||
							item.owner == collB && item.colider == collA)
						{
							isCollInfo = true;
						}
					}
					//ない場合
					if (!isCollInfo)
					{
						onCollideInfo.emplace_back(OnCollideInfo{ collA, collB });
						onCollideInfo.emplace_back(OnCollideInfo{ collB, collA });
					}
				}
			}
		}
		//チェックの必要がないなら
		if (!isOneMore)break;
	}
	//位置確定
	for (auto& coll : m_collidables)
	{
		//位置を確定
		coll->Complete();
	}
	// 当たり通知
	for (auto& collInfo : onCollideInfo)
	{
		collInfo.OnCollide();
	}
}

void Physics::Reset()
{
	m_collidables.clear();
}

std::list<std::weak_ptr<Collidable>> Physics::GetAreaXCollidable(float startX, float endX)
{
	std::list<std::weak_ptr<Collidable>> collList;
	for (auto& collidable : m_collidables)
	{
		if (collidable->GetGameTag() == GameTag::None)continue;
		auto collPos = collidable->m_rb->m_pos;
		//範囲内にいたら
		if (collPos.x > startX && collPos.x < endX)
		{
			collList.emplace_back(collidable);
		}
	}
	return collList;
}

std::list<std::weak_ptr<Collidable>> Physics::RayCast(const Vector3& startPos, const Vector3& endPos)
{
	std::list<std::weak_ptr<Collidable>> collList;
	for (auto& collidable : m_collidables)
	{
		if (collidable->GetGameTag() == GameTag::None)continue;
		if (collidable->GetGameTag() == GameTag::Attack)continue;
		if (collidable->GetGameTag() == GameTag::Area)continue;
		if (collidable->m_isTrigger)continue;
		if (collidable->m_isThrough)continue;
		auto shape = collidable->GetShape();
		if (shape == Shape::None)continue;
		//球が当たっているなら
		if (shape == Shape::Sphere)
		{
			if (HitCheck_Line_Sphere(startPos.ToDxLibVector(), endPos.ToDxLibVector(),
				collidable->m_rb->m_pos.ToDxLibVector(), std::dynamic_pointer_cast<SphereCollider>(collidable->m_collisionData)->GetRadius()))
			{
				collList.emplace_back(collidable);
			}
		}
		//カプセル
		else if(shape == Shape::Capsule)
		{
			auto cap = std::dynamic_pointer_cast<CapsuleCollider>(collidable->m_collisionData);
			if (HitCheck_Capsule_Capsule(startPos.ToDxLibVector(), endPos.ToDxLibVector(), 0.0f,
				collidable->m_rb->m_pos.ToDxLibVector(), cap->GetEndPos().ToDxLibVector(), cap->GetRadius()))
			{
				collList.emplace_back(collidable);
			}
		}
		//ポリゴン
		else if(shape == Shape::Polygon)
		{
			auto hitData = MV1CollCheck_Line(std::dynamic_pointer_cast<PolygonCollider>(collidable->m_collisionData)->GetModelHandle(), -1,
				startPos.ToDxLibVector(), endPos.ToDxLibVector());
			if (hitData.HitFlag)
			{
				collList.emplace_back(collidable);
			}
		}
	}
	return collList;
}

Vector3 Physics::GetCameraRatCastNearEndPos(const Vector3& targetPos, const Vector3& cameraPos)
{
	auto hitColls = RayCast(targetPos, cameraPos);
	float shortDis = 1000000.0f;
	Vector3 hitPos = cameraPos;
	for (auto& hitColl : hitColls)
	{
		if (hitColl.expired())continue;
		auto pHitColl = hitColl.lock();
		//ポリゴンかチェック
		if (pHitColl->GetShape() != Shape::Polygon)continue;
		if (pHitColl->m_isThrough)continue;
		if (pHitColl->m_isTrigger)continue;
		MV1_COLL_RESULT_POLY result = MV1CollCheck_Line(std::dynamic_pointer_cast<PolygonCollider>(pHitColl->m_collisionData)->GetModelHandle(), -1,
			targetPos.ToDxLibVector(), cameraPos.ToDxLibVector());
		if (!result.HitFlag)continue;
		//最短かどうか
		float dis = (Vector3(result.HitPosition) - targetPos).Magnitude();
		if (shortDis > dis)
		{
			shortDis = dis;
			hitPos = result.HitPosition;
		}
	}
	return hitPos;
}

void Physics::InitTimeScale()
{
	float timeScale = Application::GetInstance().GetTimeScale();
	for (auto& collidable : m_collidables)
	{
		//自分のタイムスケールを優先しないなら
		if (!collidable->m_rb->IsMyTimeScale())
		{
			//グローバルのタイムスケールをセット
			collidable->SetTimeScale(timeScale);
		}
	}
}

void Physics::Gravity()
{
	auto& app = Application::GetInstance();
	
	for (auto& collidable : m_collidables)
	{
		//重力を受けるか
		if (!collidable->m_rb->m_isGravity)continue;
		auto rb = collidable->m_rb;
		//地上にいるときと空中にいるときで重力の大きさを変える
		auto gravity = kGroundGravity;
		if (!collidable->IsFloor())//地上にいない場合
		{
			gravity = kAirGravity * rb->GetMyTimeScale();
		}
		//時間補正付きで重力を加える
		rb->m_vec += gravity;
	}
}