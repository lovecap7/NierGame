#include "PlayerHPUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>

namespace
{

}

PlayerHPUI::PlayerHPUI(std::shared_ptr<CharaStatus> charaStatus):
	m_playerStatus(charaStatus),
	m_nowValueHandle(-1),
	m_backValueHandle(-1),
	m_damageValueHandle(-1),
	m_healValueHandle(-1),
	m_damageFrame(0),
	m_damageValueRate(0.0f),
	m_healFrame(0),
	m_healValueRate(0.0f)
{
	auto& assetManager = AssetManager::GetInstance();
	m_nowValueHandle = assetManager.GetImageHandle(L"Player/HPValue");
	m_backValueHandle = assetManager.GetImageHandle(L"Player/HPValueBack");
	m_damageValueHandle = assetManager.GetImageHandle(L"Player/HPValueDamage");
	m_healValueHandle = assetManager.GetImageHandle(L"Player/HPValueHeal");
	m_beforeNowHP = m_playerStatus->GetNowHP();
	m_damageValueRate = m_playerStatus->GetHPRate();
	m_healValueRate = m_playerStatus->GetHPRate();
}

PlayerHPUI::~PlayerHPUI()
{
}

void PlayerHPUI::Update()
{
	float nowHP = m_playerStatus->GetNowHP();
	//体力が前のフレームより低かったら
	if (nowHP < m_beforeNowHP)
	{
		//ダメージを受けた
		m_damageFrame = 120;
	}
	else if (nowHP > m_beforeNowHP)
	{
		//回復
		m_healFrame = 120;
	}
	//現在の体力を更新
	m_beforeNowHP = nowHP;

	//ダメージを受けた
	if (m_damageFrame > 0)
	{
		--m_damageFrame;
	}
	else
	{
		//現在の体力に合わせる
		m_damageValueRate = MathSub::Lerp(m_damageValueRate, m_playerStatus->GetHPRate(), 0.1f);
	}

	//体力が回復
	if (m_healFrame > 0)
	{
		--m_healFrame;
	}
	else
	{
		//現在の体力に合わせる
		m_healValueRate = MathSub::Lerp(m_healValueRate, m_playerStatus->GetHPRate(), 0.1f);
	}
}

void PlayerHPUI::Draw() const
{
	DrawExtendGraphF(100.0f, 100.0f, 100.0f + 250.0f, 100.0f + 20.0f, m_backValueHandle, true);
	DrawExtendGraphF(100.0f, 100.0f, 100.0f + (250.0f * m_damageValueRate), 100.0f + 20.0f, m_damageValueHandle, true);
	DrawExtendGraphF(100.0f, 100.0f, 100.0f + (250.0f * m_healValueRate), 100.0f + 20.0f, m_healValueHandle, true);
	DrawExtendGraphF(100.0f, 100.0f, 100.0f + (250.0f * m_playerStatus->GetHPRate()), 100.0f + 20.0f, m_nowValueHandle, true);
}
