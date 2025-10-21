#include "CharacterBase.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/CSV/CSVDataLoader.h"
#include "../../../General/CSV/CSVData.h"
#include "../../../General/CSV/AttackData.h"
#include <cassert>
CharacterBase::CharacterBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, Shape shape, std::weak_ptr<ActorManager> pActorManager) :
	Actor(actorData,shape,pActorManager),
	m_state(),
	m_charaStatus()
{
	m_charaStatus = std::make_shared<CharaStatus>(charaStatusData);
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
	//UŒ‚ƒ}ƒl[ƒWƒƒ[
	if (actorManager->GetAttackManager().expired())return;
	auto attackManager = actorManager->GetAttackManager().lock();

	attackManager->Entry(attack);
}


std::shared_ptr<AttackData> CharacterBase::GetAttackData(std::wstring attackName) const
{
	std::shared_ptr<AttackData> attackData;

	//’T‚·
	for (auto& data : m_attackDatas)
	{
		//ðŒ‚É‡‚¤‚à‚Ì‚ª‚ ‚Á‚½‚ç
		if (data->m_name == attackName)
		{
			attackData = data;
			break;
		}
	}

	assert(attackData);

	return attackData;
}


void CharacterBase::InitAttackData(std::shared_ptr<CSVDataLoader> csvLoader, std::string path)
{
	auto datas = csvLoader->LoadCSV(path.c_str());
	//“o˜^
	for (auto& data : datas)
	{
		std::shared_ptr<AttackData> attackData = std::make_shared<AttackData>(data);
		m_attackDatas.emplace_back(attackData);
	}
}
