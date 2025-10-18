#include "CharacterBase.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../../Attack/AttackManager.h"

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
	//攻撃マネージャー
	if (actorManager->GetAttackManager().expired())return;
	auto attackManager = actorManager->GetAttackManager().lock();

	attackManager->Entry(attack);
}
