#include "PlayerHPUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>

namespace
{
	//バーの変動フレーム
	constexpr int kFluctuationValueFrame = 120;
	//基準位置
	constexpr float kBasePosX = 100.0f;
	constexpr float kBasePosY = 70.0f;
	//バーの画像サイズ
	constexpr float kBarSizeX = 250.0f;
	constexpr float kBarSizeY = 20.0f;

	//ハンドル
	const std::wstring kHPValue = L"Player/HPValue";
	const std::wstring kHPValueBack = L"Player/HPValueBack";
	const std::wstring kHPValueDamage = L"Player/HPValueDamage";
	const std::wstring kHPValueHeal = L"Player/HPValueHeal";
	const std::wstring kHPBarFrame = L"Player/HPFrame";
}

PlayerHPUI::PlayerHPUI(std::shared_ptr<CharaStatus> charaStatus):
	HPUIBase(charaStatus)
{
	auto& assetManager = AssetManager::GetInstance();
	m_nowValueHandle = assetManager.GetImageHandle(kHPValue);
	m_backValueHandle = assetManager.GetImageHandle(kHPValueBack);
	m_damageValueHandle = assetManager.GetImageHandle(kHPValueDamage);
	m_healValueHandle = assetManager.GetImageHandle(kHPValueHeal);
	m_barFrameHandle = assetManager.GetImageHandle(kHPBarFrame);
	m_beforeNowHP = m_playerStatus->GetNowHP();
	m_nowHpRate = m_playerStatus->GetHPRate();
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
		m_damageFrame = kFluctuationValueFrame;
		//回復バーは体力に合わせる
		m_healValueRate = m_playerStatus->GetHPRate();
	}
	else if (nowHP > m_beforeNowHP)
	{
		//回復
		m_healFrame = kFluctuationValueFrame;
		//回復バーは体力に合わせる
		m_healValueRate = m_playerStatus->GetHPRate();
	}
	//現在の体力を更新
	m_beforeNowHP = nowHP;

	//ダメージを受けた
	UpdateDamageBar();

	//体力が回復
	UpdateHealBar();
}

void PlayerHPUI::Draw() const
{
	if (!m_isDraw)return;
	//周りのフレーム
	DrawRotaGraphF((kBasePosX + kBasePosX + kBarSizeX) * 0.5f, (kBasePosY + kBasePosY + kBarSizeY) * 0.5f, 1.0, 0.0, m_barFrameHandle, true);
	//バーの描画
	DrawExtendGraphF(kBasePosX, kBasePosY, kBasePosX + kBarSizeX, kBasePosY + kBarSizeY, m_backValueHandle, true);
	DrawExtendGraphF(kBasePosX, kBasePosY, kBasePosX + (kBarSizeX * m_damageValueRate), kBasePosY + kBarSizeY, m_damageValueHandle, true);
	DrawExtendGraphF(kBasePosX, kBasePosY, kBasePosX + (kBarSizeX * m_healValueRate), kBasePosY + kBarSizeY, m_healValueHandle, true);
	DrawExtendGraphF(kBasePosX, kBasePosY, kBasePosX + (kBarSizeX * m_nowHpRate), kBasePosY + kBarSizeY, m_nowValueHandle, true);
}
