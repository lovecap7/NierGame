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
#include "../../../Attack/AttackManager.h"
#include "../../../Camera/PlayerCamera.h"
#include "../../../../General/CSV/CSVDataLoader.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//敵キャラのパスデータ数
	constexpr int kPathNum = 5;
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

	//攻撃のキーを取得
	InitAttackKey(csvLoader, pathData[3]);

	//目の位置
	InitEyeIndex(csvLoader, pathData[4]);

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<NormalEnemy>(shared_from_this());
	m_state = std::make_shared<EnemyStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->m_collRadius);
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