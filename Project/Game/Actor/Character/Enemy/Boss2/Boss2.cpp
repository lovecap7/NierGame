#include "Boss2.h"
#include "../EnemyStateBase.h"
#include "Boss2StateIdle.h"
#include "Boss2Arm.h"
#include "../../../ActorManager.h"
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
#include "../../../../../General/CSV/ArmData.h"
#include "../../../../../General/CSV/CharaStatusData.h"
#include "../../../../UI/Enemy/NormalEnemyHPUI.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//敵キャラのパスデータ数
	constexpr int kPathNum = 6;
}

Boss2::Boss2(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	EnemyBase(actorData, charaStatusData, pActorManager)
{
	m_charaStatusData = charaStatusData;
	//ボス
	m_isBoss = true;
}
Boss2::~Boss2()
{
}
void Boss2::Init()
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

	//腕の作成
	InitArm(csvLoader, pathData);

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Boss2>(shared_from_this());
	m_state = std::make_shared<Boss2StateIdle>(thisPointer);
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

void Boss2::Update()
{
	//非活動中は更新しない
	if (!m_isActive)return;

	//カメラの当たり判定を消す(ロックオンした際にボスとの間の壁に当たらないようにするため)
	if (!m_pActorManager.expired())
	{
		auto actorManger = m_pActorManager.lock();
		if (!actorManger->GetPlayerCamera().expired())
		{
			actorManger->GetPlayerCamera().lock()->DisableColl();
		}
	}

	//攻撃を喰らった際
	if (m_charaStatus->IsHit())
	{
		m_model->ModelHit();
	}
	//攻撃クールタイム更新
	CountAttackCoolTime();
	//共通処理
	CharacterBase::Update();
	//ロックオンの位置
	UpdateLockOnViewPos();
	//目の位置更新
	UpdateEyeEffect();
	
}

void Boss2::Complete()
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


void Boss2::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
	if (!m_rightArm.expired())
	{
		m_rightArm.lock()->End();
	}
	if (!m_leftArm.expired())
	{
		m_leftArm.lock()->End();
	}
}

void Boss2::InitArm(CSVDataLoader& csvLoader, std::vector<std::wstring>& pathData)
{
	auto armStrDatas = csvLoader.LoadCSV(pathData[5].c_str());
	if (m_pActorManager.expired())return;
	auto actorManger = m_pActorManager.lock();

	for (auto& data : armStrDatas)
	{
		auto armData = std::make_shared<ArmData>(data);
		//データ
		std::shared_ptr<ActorData> actorData = std::make_shared<ActorData>();
		std::shared_ptr<CharaStatusData> charaStatus = std::make_shared<CharaStatusData>();
		actorData->SetCollRadius(armData->GetRadius());
		actorData->SetGameTag(GameTag::Enemy);
		actorData->SetIsTrough(false);
		actorData->SetPriority(Priority::Static);
		charaStatus->SetID(m_actorData->GetActorID());
		charaStatus->SetHP(armData->GetHP());
		charaStatus->SetDF(armData->GetDF());
		charaStatus->SetAR(armData->GetArmor());
		if (armData->GetName() == L"Right")
		{
			actorData->SetActorID(static_cast<int>(ArmIndex::RightArm));
			auto rightArm = std::make_shared<Boss2Arm>(actorData, charaStatus, m_pActorManager);
			rightArm->SetArmIndex(m_model->GetModelHandle(), armData->GetStartIndex(), armData->GetEndIndex());
			m_rightArm = rightArm;
			actorManger->Entry(rightArm);
		}
		else if (armData->GetName() == L"Left")
		{
			actorData->SetActorID(static_cast<int>(ArmIndex::LeftArm));
			auto leftArm = std::make_shared<Boss2Arm>(actorData, charaStatus, m_pActorManager);
			leftArm->SetArmIndex(m_model->GetModelHandle(), armData->GetStartIndex(), armData->GetEndIndex());
			m_leftArm = leftArm;
			actorManger->Entry(leftArm);
		}
		if (!m_rightArm.expired() && !m_leftArm.expired())
		{
			break;
		}
	}
}
