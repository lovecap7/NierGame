#include "BossHPUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Game.h"
#include "../../Actor/Character/Enemy/EnemyBase.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//バーの変動フレーム
	constexpr int kFluctuationValueFrame = 120;
	//バーの画像サイズ
	constexpr float kBarSizeX = 20.0f;
	constexpr float kBarSizeY = 200.0f;
	//体力に変動があった場合10秒間表示
	constexpr int kChangeHPFrame = 60 * 10;

	//背景
	constexpr float kBackBarSizeX = 24.0f;
	constexpr float kBackBarSizeY = 204.0f;

	//位置
	constexpr float kPosX = Game::kScreenWidth - kBarSizeX - 10;
	constexpr float kPosY = Game::kScreenCenterY;

	//ハンドル
	const std::wstring kHPValue = L"Enemy/BossHPValue";
	const std::wstring kHPValueBack = L"Enemy/BossHPValueBack";
	const std::wstring kHPValueDamage = L"Enemy/BossHPValueDamage";
	const std::wstring kHPValueHeal = L"Enemy/BossHPValueHeal";
}

BossHPUI::BossHPUI(std::shared_ptr<CharaStatus> charaStatus, std::weak_ptr<EnemyBase> pEnemy) :
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

BossHPUI::~BossHPUI()
{
}

void BossHPUI::Update()
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

}

void BossHPUI::Draw() const
{
	if (m_pEnemy.expired())return;
	auto enemy = m_pEnemy.lock();
	if (!enemy->IsActive())return;
	//バーの描画
	DrawRectGraphF(kPosX, kPosX, 0, 0, kBackBarSizeX, kBackBarSizeY, m_backValueHandle, true);
	//バーの描画
	DrawRectGraphF(kPosX, kPosY, 0, 0, kBarSizeX , kBarSizeY * -m_damageValueRate, m_damageValueHandle, true);
	DrawRectGraphF(kPosX, kPosY, 0, 0, kBarSizeX , kBarSizeY * -m_healValueRate, m_healValueHandle, true);
	DrawRectGraphF(kPosX, kPosY, 0, 0, kBarSizeX , kBarSizeY * -m_nowHpRate, kBarSizeY, m_nowValueHandle, true);
}
