#include "Boss1.h"
#include "../EnemyStateIdle.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/CharaStatus.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../Attack/AttackManager.h"
#include "../../../../Camera/PlayerCamera.h"
#include "../../../../../General/CSV/CSVDataLoader.h"
#include "../../../../../General/CSV/EnemyAttackKeyData.h"
#include "../../../../UI/Enemy/NormalEnemyHPUI.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//敵キャラのパスデータ数
	constexpr int kPathNum = 5;
}

Boss1::Boss1(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	EnemyBase(actorData, charaStatusData, pActorManager)
{
	//ボス
	m_isBoss = true;
}
Boss1::~Boss1()
{
}
void Boss1::Init()
{
	//必要なパスを取得
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto pathData = csvLoader.LoadCSV(m_actorData->GetCSVPathData().c_str()).front()->GetData();

	assert(pathData.size() >= kPathNum);
	//共通初期化
	CharacterBase::Init(pathData[0].c_str(), pathData[1].c_str(), pathData[2].c_str());

	//攻撃のキーを取得
	InitAttackKey(csvLoader, pathData[3]);

	//目の位置
	InitEyeIndex(csvLoader, pathData[4]);

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Boss1>(shared_from_this());
	m_state = std::make_shared<EnemyStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->GetCollRadius());

	//体力UI
	auto enemyHPUI = std::make_shared<NormalEnemyHPUI>(m_charaStatus, thisPointer);
	enemyHPUI->Init();
	//体力回復
	m_charaStatus->FullRecovery();

	//位置
	m_rb->SetPos(m_actorData->GetPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->GetCollHeight();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//移動量リセット
	m_rb->SetVec(Vector3::Zero());

	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();
}


void Boss1::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->GetCollHeight();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();

	if (m_charaStatus->IsHit())
	{
#if _DEBUG
		printf("敵の残りの体力 : %d\n", m_charaStatus->GetNowHP());
#endif
	}
}


void Boss1::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
}

std::shared_ptr<AttackData> Boss1::GetRandomAttack(std::vector<std::wstring> keys) const
{
	//近接攻撃の場合
	if (m_meleeAttackKeys == keys)
	{
		//プレイヤーの高さから攻撃を選択
		if (m_targetInfo.m_isFound)
		{
			//自分の中心より高い場合
			if (GetCenterPos().y < m_targetInfo.m_pTarget.lock()->GetPos().y)
			{
				//パンチ
				return GetAttackData(keys[0]);
			}
			else
			{
				//パンチ攻撃以外
				return GetAttackData(keys[MyMath::GetRand(1, static_cast<int>(keys.size()) - 1)]);
			}
		}
	}
	int index = MyMath::GetRand(0, static_cast<int>(keys.size()) - 1);
	return GetAttackData(keys[index]);
}
