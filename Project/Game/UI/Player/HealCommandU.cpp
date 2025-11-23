#include "HealCommandU.h"
#include "../../Actor/Character/Player/Player.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include <DxLib.h>

namespace
{
	//背景位置
	constexpr int kBackPosX = 60;
	constexpr int kBackPosY = Game::kScreenHeight - 150;
	//文字位置
	constexpr int kTextPosX = kBackPosX + 28;
	constexpr int kTextPosY = kBackPosY + 16;

	//移動角度
	constexpr float kMoveAngleSpeed = 10.0f;
	constexpr float kMoveY = 5.0f;
	constexpr float kMoveAngleMax = 180.0f;

	//点滅フレーム
	constexpr int kPinchBlinkFrame = 20;

	//ハンドル
	const std::wstring kBack = L"Player/HealCommandUI";
}

HealCommandU::HealCommandU(std::weak_ptr<Player> pPlayer):
	UIBase(),
	m_pPlayer(pPlayer),
	m_backHandle(-1),
	m_fontHandle(-1),
	m_beforeFullRecoveryNum(0),
	m_moveAngle(kMoveAngleMax),
	m_pinchBlinkCount(0)
{
	auto& assetManager = AssetManager::GetInstance();
	m_backHandle = assetManager.GetImageHandle(kBack);
	m_fontHandle = assetManager.GetFontHandle(AssetManager::Font(AssetManager::FontType::NotoSansJP, AssetManager::FontSize::Size20));
	if(!m_pPlayer.expired())
	{
		m_beforeFullRecoveryNum = m_pPlayer.lock()->GetFullRecoveryNum();
	}
}

HealCommandU::~HealCommandU()
{
}

void HealCommandU::Update()
{
	if (m_pPlayer.expired())return;
	auto player = m_pPlayer.lock();
	int currentFullRecoveryNum = player->GetFullRecoveryNum();
	//回復可能回数が変化したら
	if (m_beforeFullRecoveryNum != currentFullRecoveryNum)
	{
		m_moveAngle = 0.0f;

		m_beforeFullRecoveryNum = currentFullRecoveryNum;
	}
	if( m_moveAngle < kMoveAngleMax)
	{
		m_moveAngle += kMoveAngleSpeed;
	}
	else
	{
		m_moveAngle = kMoveAngleMax;
	}

	if (player->GetCharaStatus()->IsPinchHP())
	{
		++m_pinchBlinkCount;
		if(m_pinchBlinkCount >= kPinchBlinkFrame)
		{
			m_pinchBlinkCount = 0;
		}
	}
}

void HealCommandU::Draw() const
{
	if (!m_isDraw)return;
	if (m_pPlayer.expired())return;
	auto player = m_pPlayer.lock();

	//回復した際に動かす
	int moveY = cosf(m_moveAngle * MyMath::DEG_2_RAD) * kMoveY;

	DxLib::DrawGraph(kBackPosX, kBackPosY + moveY, m_backHandle, true);
	if(player->GetCharaStatus()->IsPinchHP())
	{
		//ピンチの時は点滅
		if (m_pinchBlinkCount >= kPinchBlinkFrame * 0.5f)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			DxLib::DrawGraph(kBackPosX, kBackPosY + moveY, m_backHandle, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	const std::wstring healCommand = L"全回復（残り " + std::to_wstring(player->GetFullRecoveryNum()) + L" )";
	DrawStringToHandle(kTextPosX, kTextPosY + moveY, healCommand.c_str(), 0x4e4b42, m_fontHandle);
}
