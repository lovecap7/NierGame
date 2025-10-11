#include "CharacterBase.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Model.h"

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
