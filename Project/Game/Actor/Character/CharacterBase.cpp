#include "CharacterBase.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/CSV/CSVDataLoader.h"
#include "../../../General/CSV/CSVData.h"
#include "../../../General/CSV/AttackData.h"
#include "CharacterStateBase.h"
#include <cassert>
CharacterBase::CharacterBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, Shape shape, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData,shape,pActorManager),
	m_state(),
	m_charaStatus()
{
	m_charaStatus = std::make_shared<CharaStatus>(charaStatusData);
}

void CharacterBase::Init(std::wstring animPath, std::wstring attackPath)
{
	//Physicsに登録
	Collidable::Init();
	//CSVを読み込む
	auto& csvLoader = CSVDataLoader::GetInstance();
	//アニメーションデータ
	InitAnimData(csvLoader, animPath);
	//攻撃データ
	InitAttackData(csvLoader, attackPath);
}

void CharacterBase::Update()
{
	//状態に合わせた更新
	m_state->Update();
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();

	//状態のリセット
	m_charaStatus->InitHitState();
}

std::shared_ptr<CharaStatus> CharacterBase::GetCharaStatus() const
{
	std::shared_ptr<CharaStatus> status;
	if (m_charaStatus)
	{
		status = m_charaStatus;
	}
	return status;
}

void CharacterBase::SetAttack(std::shared_ptr<AttackBase> attack)
{
	if (m_pActorManager.expired())return;
	auto actorManager = m_pActorManager.lock();
	//攻撃マネージャー
	if (actorManager->GetAttackManager().expired())return;
	auto attackManager = actorManager->GetAttackManager().lock();

	attackManager->Entry(attack);
}


std::shared_ptr<AttackData> CharacterBase::GetAttackData(std::wstring attackName) const
{
	std::shared_ptr<AttackData> attackData;

	//探す
	for (auto& data : m_attackDatas)
	{
		//条件に合うものがあったら
		if (data->m_name == attackName)
		{
			attackData = data;
			break;
		}
	}

	assert(attackData);

	return attackData;
}

std::string CharacterBase::GetAnim(std::wstring state, std::string path,AnimData::WeaponType type)const
{
	//探す
	for (auto& data : m_animDatas)
	{
		//条件に合うものがあったら
		if (data->m_stateName == state && data->m_weaponType == type)
		{
			path += data->m_animName;
			break;
		}
	}

	return path;
}
void CharacterBase::InitAttackData(CSVDataLoader& csvLoader, std::wstring path)
{
	auto datas = csvLoader.LoadCSV(path.c_str());
	//登録
	for (auto& data : datas)
	{
		std::shared_ptr<AttackData> attackData = std::make_shared<AttackData>(data);
		m_attackDatas.emplace_back(attackData);
	}
}

void CharacterBase::InitAnimData(CSVDataLoader& csvLoader,std::wstring path)
{
	auto datas = csvLoader.LoadCSV(path.c_str());
	//登録
	for (auto& data : datas)
	{
		std::shared_ptr<AnimData> animData = std::make_shared<AnimData>(data);
		m_animDatas.emplace_back(animData);
	}
}
