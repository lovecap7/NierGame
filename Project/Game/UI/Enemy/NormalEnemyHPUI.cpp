#include "NormalEnemyHPUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CharaStatus.h"
#include "../../Actor/Character/Enemy/EnemyBase.h"
#include <DxLib.h>
#include <cassert>

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
}

NormalEnemyHPUI::NormalEnemyHPUI(std::shared_ptr<CharaStatus> charaStatus, std::weak_ptr<EnemyBase> pEnemy) :
	HPUIBase(charaStatus),
	m_pEnemy(pEnemy)

{
	auto& assetManager = AssetManager::GetInstance();
	m_nowValueHandle = assetManager.GetImageHandle(kHPValue);
	m_backValueHandle = assetManager.GetImageHandle(kHPValueBack);
	m_damageValueHandle = assetManager.GetImageHandle(kHPValueDamage);
	m_healValueHandle = assetManager.GetImageHandle(kHPValueHeal);
	m_beforeNowHP = m_playerStatus->GetNowHP();
	m_nowHpRate = m_playerStatus->GetHPRate();
	m_damageValueRate = m_playerStatus->GetHPRate();
	m_healValueRate = m_playerStatus->GetHPRate();
}

NormalEnemyHPUI::~NormalEnemyHPUI()
{
}

void NormalEnemyHPUI::Update()
{
	if (m_pEnemy.expired())return;
	auto enemy = m_pEnemy.lock();
	if (!enemy->IsActive())return;

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

	//座標更新
	m_enemyViewPos = ConvWorldPosToScreenPos(enemy->GetHeadPos().ToDxLibVector());
}

void NormalEnemyHPUI::Draw() const
{
	if (m_pEnemy.expired())return;
	if (!m_pEnemy.lock()->IsActive())return;

	auto uiPos = m_enemyViewPos;
	uiPos.x -= (kBarSizeX * 0.5f);
	//バーの描画
	DrawRectGraphF(uiPos.x, uiPos.y, 0, 0, kBarSizeX, kBarSizeY, m_backValueHandle, true);
	DrawRectGraphF(uiPos.x, uiPos.y, 0, 0, kBarSizeX * m_damageValueRate, kBarSizeY, m_damageValueHandle, true);
	DrawRectGraphF(uiPos.x, uiPos.y, 0, 0, kBarSizeX * m_healValueRate, kBarSizeY, m_healValueHandle, true);
	DrawRectGraphF(uiPos.x, uiPos.y, 0, 0, kBarSizeX * m_nowHpRate, kBarSizeY, kBarSizeY, m_nowValueHandle, true);
}
