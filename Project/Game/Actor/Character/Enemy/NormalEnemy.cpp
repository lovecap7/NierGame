#include "NormalEnemy.h"
#include "EnemyStateIdle.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../ActorManager.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Camera/PlayerCamera.h"
#include "../../../../General/CSV/CSVDataLoader.h"
#include "../../../../General/CSV/CSVData.h"
#include "../../../../General/CSV/AttackData.h"
#include "../../../../General/CSV/EnemyAttackKeyData.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//敵キャラのパスデータ数
	constexpr int kPathNum = 4;
}

NormalEnemy::NormalEnemy(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager):
	EnemyBase(actorData, charaStatusData, pActorManager)
{
}
NormalEnemy::~NormalEnemy()
{
}
void NormalEnemy::Init()
{
	//必要なパスを取得
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto pathData = csvLoader.LoadCSV(m_actorData->m_csvPathData.c_str()).front()->GetData();

	assert(pathData.size() >= kPathNum);
	//共通初期化
	CharacterBase::Init(pathData[0].c_str(), pathData[1].c_str(), pathData[2].c_str());

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<NormalEnemy>(shared_from_this());
	m_state = std::make_shared<EnemyStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->m_collRadius);

	//攻撃のキーを取得
	auto oriAttackKeys = csvLoader.LoadCSV(pathData[3].c_str());
	for (auto& data : oriAttackKeys)
	{
		std::shared_ptr<EnemyAttackKeyData> attackKeyData = std::make_shared<EnemyAttackKeyData>(data);
		//近接攻撃
		if (attackKeyData->m_attackRangeType == EnemyAttackKeyData::AttackRangeType::Melee)
		{
			m_meleeAttackKeys.emplace_back(attackKeyData->m_attackKeyName);
		}
		//遠距離攻撃
		else if (attackKeyData->m_attackRangeType == EnemyAttackKeyData::AttackRangeType::LongRange)
		{
			m_longRangeAttackKeys.emplace_back(attackKeyData->m_attackKeyName);
		}
	}
}

void NormalEnemy::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->m_collHeight;
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


void NormalEnemy::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
}
