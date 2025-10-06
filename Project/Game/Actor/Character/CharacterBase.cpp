#include "CharacterBase.h"
#include "../../../General/Model.h"
#include "../../../General/Sound/SoundManager.h"
#include "../../../General/HitPoints.h"
#include "../../../General/AttackPoints.h"

CharacterBase::CharacterBase(Shape shape) :
	Actor(shape)
{
}

std::weak_ptr<SE>  CharacterBase::CharacterOnceSE(std::string name)
{
	return SoundManager::GetInstance().PlayOnceSE(m_soundHandles.at(name));
}

std::weak_ptr<SE>  CharacterBase::CharacterLoopSE(std::string name)
{
	return SoundManager::GetInstance().PlayLoopSE(m_soundHandles.at(name));
}

std::weak_ptr<Voice> CharacterBase::CharacterVC(std::string name)
{
	return SoundManager::GetInstance().PlayVC(m_soundHandles.at(name));
}

void CharacterBase::SetCharaData(ActorData data)
{
	//ó‚¯æ‚Á‚½ƒf[ƒ^‚ğ“ü‚ê‚Ä‚¢‚­
	if (m_model)
	{
		//‘å‚«‚³
		m_model->SetScale(data.scale);
		//‰ñ“]
		m_model->SetRot(data.rot);
		//ù‰ñ‘¬“x
		m_model->SetRotSpeed(data.modelRotateSpeed);
	}
	//‘Ì—Í
	if (m_hitPoints)
	{
		m_hitPoints->SetHp(data.hp);
		m_hitPoints->SetArmor(data.armor);
	}
	//UŒ‚—Í
	if (m_attackPoints)
	{
		m_attackPoints->SetAttackPower(data.attackPower);
	}
	//ƒOƒ‹[ƒv
	SetGroupTag(data.gropeTag);
	//’TõêŠ‚Ì”ÍˆÍ
	SetSearchPlaceRang(data.searchPlaceRang);
	//‹–ìŠp
	m_viewingAngle = data.viewingAngle;
	//õ“G‹——£
	m_searchDistance = data.searchDistance;
}

void CharacterBase::EndSound()
{
	//íœ
	for (const auto& [key, value] : m_soundHandles) {
		DeleteSoundMem(value);
	}
}