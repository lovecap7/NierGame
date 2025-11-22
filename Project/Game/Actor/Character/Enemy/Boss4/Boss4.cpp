#include "Boss4.h"
#include "Boss4StateIdle.h"
#include "../EnemyStateIdle.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/CharaStatus.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/NormalEffect.h"
#include "../../../../Attack/AttackManager.h"
#include "../../../../Camera/PlayerCamera.h"
#include "../../../../../General/CSV/CSVDataLoader.h"
#include "../../../../../General/CSV/EnemyAttackKeyData.h"
#include "../../../../UI/Enemy/BossHPUI.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//敵キャラのパスデータ数
	constexpr int kFirstPhasePathNum = 6;
	constexpr int kSecondPhasePathNum = 5;
	//オーラ
	const std::wstring kAuraEffect = L"Aura";}

Boss4::Boss4(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	EnemyBase(actorData, charaStatusData, pActorManager),
	m_isSecondPhase(false),
	m_secondPhaseDataPath()
{
	//ボス
	m_isBoss = true;
}
Boss4::~Boss4()
{
}
void Boss4::Init()
{
	//必要なパスを取得
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto pathData = csvLoader.LoadCSV(m_actorData->GetCSVPathData().c_str()).front()->GetData();

	assert(pathData.size() >= kFirstPhasePathNum);
	//共通初期化
	CharacterBase::Init(pathData[0].c_str(), pathData[1].c_str(), pathData[2].c_str());

	//攻撃のキーを取得
	InitAttackKey(csvLoader, pathData[3]);

	//目の位置
	InitEyeIndex(csvLoader, pathData[4]);

	//第二形態のデータパス
	m_secondPhaseDataPath = pathData[5];

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Boss4>(shared_from_this());
	m_state = std::make_shared<Boss4StateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->GetCollRadius());

	//体力UI
	auto enemyHPUI = std::make_shared<BossHPUI>(m_charaStatus, thisPointer);
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


	if (!m_auraEffect.expired())
	{
		m_auraEffect.lock()->Delete();
	}
	//オーラエフェクト
	m_auraEffect = EffekseerManager::GetInstance().CreateEffect(GetEffectPath(kAuraEffect), m_rb->m_pos);

	//第二形態
	m_isSecondPhase = false;
}


void Boss4::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->GetCollHeight();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();

	//オーラエフェクトの位置更新
	if (!m_auraEffect.expired())
	{
		m_auraEffect.lock()->SetPos(m_rb->m_pos);
	}

	if (m_charaStatus->IsHit())
	{
#if _DEBUG
		printf("敵の残りの体力 : %d\n", m_charaStatus->GetNowHP());
#endif
	}
}


void Boss4::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
}

void Boss4::ChangeSecondPhase()
{
	//必要なパスを取得
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto pathData = csvLoader.LoadCSV(m_secondPhaseDataPath.c_str()).front()->GetData();

	assert(pathData.size() >= kSecondPhasePathNum);
	//共通初期化
	CharacterBase::Init(pathData[0].c_str(), pathData[1].c_str(), pathData[2].c_str());

	//攻撃のキーを取得
	InitAttackKey(csvLoader, pathData[3]);

	//目の位置
	InitEyeIndex(csvLoader, pathData[4]);
	
	if (!m_auraEffect.expired())
	{
		m_auraEffect.lock()->Delete();
	}
	//オーラエフェクト
	m_auraEffect = EffekseerManager::GetInstance().CreateEffect(GetEffectPath(kAuraEffect), m_rb->m_pos);

	//体力回復
	m_charaStatus->FullRecovery();

	//無敵解除
	m_charaStatus->SetIsNoDamage(false);

	//第二形態
	m_isSecondPhase = true;
}
