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
#include "../../../../../General/AssetManager.h"
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
	constexpr int kPathNum = 7;
	//状態に合わせたモデル数
	constexpr int kArmStateModelNum = 3;
	//腕が破壊されたときのダメージ
	constexpr float kDestroyDamageRate = 0.4f;
}

Boss2::Boss2(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	EnemyBase(actorData, charaStatusData, pActorManager),
	m_noRightHandle(-1),
	m_noLeftHandle(-1),
	m_noBothHandle(-1),
	m_isInitNoRight(false),
	m_isInitNoLeft(false),
	m_isInitNoBoth(false)
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
	InitArm(csvLoader, pathData[5]);

	//腕の状態に合わせたモデル
	InitArmStateModel(csvLoader, pathData[6]);

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

	//腕チェック
	CheckArmState();


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
	m_rb->SetVec(Vector3::Zero());
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
	MV1DeleteModel(m_noRightHandle);
	MV1DeleteModel(m_noLeftHandle);
	MV1DeleteModel(m_noBothHandle);
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

void Boss2::InitArm(CSVDataLoader& csvLoader, std::wstring pathData)
{
	auto armStrDatas = csvLoader.LoadCSV(pathData.c_str());
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
		actorData->SetIsTrigger(false);
		actorData->SetPriority(Priority::Static);
		charaStatus->SetID(m_actorData->GetActorID());
		charaStatus->SetHP(armData->GetHP());
		charaStatus->SetDF(armData->GetDF());
		charaStatus->SetAR(armData->GetArmor());
		if (armData->GetName() == L"Right")
		{
			actorData->SetActorID(static_cast<int>(ArmIndex::RightArm));
			auto rightArm = std::make_shared<Boss2Arm>(actorData, charaStatus, m_pActorManager);
			rightArm->SetArmModel(m_model->GetModelHandle());
			rightArm->SetArmIndex(armData->GetStartIndex(), armData->GetEndIndex());
			m_rightArm = rightArm;
			actorManger->Entry(rightArm);
		}
		else if (armData->GetName() == L"Left")
		{
			actorData->SetActorID(static_cast<int>(ArmIndex::LeftArm));
			auto leftArm = std::make_shared<Boss2Arm>(actorData, charaStatus, m_pActorManager);
			leftArm->SetArmModel(m_model->GetModelHandle());
			leftArm->SetArmIndex(armData->GetStartIndex(), armData->GetEndIndex());
			m_leftArm = leftArm;
			actorManger->Entry(leftArm);
		}
		if (!m_rightArm.expired() && !m_leftArm.expired())
		{
			break;
		}
	}
}

void Boss2::InitArmStateModel(CSVDataLoader& csvLoader, std::wstring pathData)
{
	auto modelPaths = csvLoader.LoadCSV(pathData.c_str());
	auto& assetManager = AssetManager::GetInstance();
	auto modelDatas = modelPaths.front()->GetData();
	if (modelDatas.size() >= kArmStateModelNum)
	{
		m_noRightHandle = assetManager.GetModelHandle(modelDatas[0]);
		m_noLeftHandle = assetManager.GetModelHandle(modelDatas[1]);
		m_noBothHandle = assetManager.GetModelHandle(modelDatas[2]);
	}
	m_isInitNoRight = false;
	m_isInitNoLeft = false;
	m_isInitNoBoth = false;
}


void Boss2::CheckArmState()
{
	bool isNoRight = IsDestroyedRightArm();
	bool isNoLeft = IsDestroyedLeftArm();
	if (!m_charaStatus)return;
	int damage = m_charaStatus->GetNowHP() - (m_charaStatus->GetMaxHP() * kDestroyDamageRate);
	if (isNoLeft && isNoRight)
	{
		if (!m_isInitNoBoth)
		{
			//本体にダメージ
			m_charaStatus->SetNowHP(damage);
			//遠距離攻撃のみを可能に
			m_charaStatus->SetMeleeAttackRange(0.0f);
			//モデル初期化
			m_model->SetModelHandle(m_noBothHandle);
			m_isInitNoBoth = true;
			m_isInitNoLeft = true;
			m_isInitNoRight = true;
		}
	}
	else if (isNoRight)
	{
		if (!m_isInitNoRight)
		{
			//本体にダメージ
			m_charaStatus->SetNowHP(damage);
			//モデル初期化
			m_model->SetModelHandle(m_noRightHandle);
			m_leftArm.lock()->SetArmModel(m_noRightHandle);
			m_isInitNoRight = true;
		}
	}
	else if (isNoLeft)
	{
		if (!m_isInitNoLeft)
		{
			//本体にダメージ
			m_charaStatus->SetNowHP(damage);
			//モデル初期化
			m_model->SetModelHandle(m_noLeftHandle);
			m_rightArm.lock()->SetArmModel(m_noLeftHandle);
			m_isInitNoLeft = true;
		}
	}
}
