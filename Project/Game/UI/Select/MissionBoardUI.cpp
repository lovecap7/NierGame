#include "MissionBoardUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include "../../../General/StringUtil.h"
#include "../../../General/CSV/MissionBoardUIData.h"
#include "../../../General/SaveDataManager.h"
#include "../../../General/CSV/ClearSaveData.h"

namespace
{
	const std::wstring kMissionBoardPath = L"Select/MissionBoard";
	const std::wstring kMissionBoardShadowPath = L"Select/MissionBoard_Shadow";
	const std::wstring kMissionBoardClearPath = L"Select/MissionBoardClear";
	const std::wstring kScoreBoardPath = L"Select/ScoreBoard";
	const std::wstring kScoreBoardShadowPath = L"Select/ScoreBoard_Shadow";

	//座標
	constexpr float kMissionBoardPosX = Game::kScreenCenterX + 200.0f;
	constexpr float kMissionBoardPosY = Game::kScreenCenterY;
	constexpr float kScoreBoardPosX = kMissionBoardPosX + 300.0f;
	constexpr float kScoreBoardPosY = kMissionBoardPosY - 100.0f;

	//影
	constexpr float kMissionBoardShadowOffsetX = 10.0f;
	constexpr float kMissionBoardShadowOffsetY = 10.0f;
	constexpr float kScoreBoardPosShadowOffsetX = 10.0f;
	constexpr float kScoreBoardPosShadowOffsetY = 10.0f;

	//テキスト座標
	constexpr float kMissionBoardTextPosX = kMissionBoardPosX - 250.0f;
	constexpr float kMissionBoardTextPosY = kMissionBoardPosY + 20.0f;

	//タイトル座標
	constexpr float kMissionBoardTitlePosX = kMissionBoardPosX;
	constexpr float kMissionBoardTitlePosY = kMissionBoardPosY - 268.0f;
}

MissionBoardUI::MissionBoardUI(int size, std::vector<std::shared_ptr<MissionBoardUIData>> datas):
	m_index(0),
	m_backHandle(-1),
	m_backShadowHandle(-1),
	m_textFontHandle(-1),
	m_titleFontHandle(-1),
	m_clearMarkHandle(-1),
	m_scoreBoardHandle(-1),
	m_scoreBoardShadowHandle(-1),
	m_datas(datas)
{
	auto& assetManager = AssetManager::GetInstance();
	//ハンドルロード
	for(int i = 0; i < size; ++i)
	{
		m_handles[i] = assetManager.GetImageHandle(kMissionBoardPath.c_str() + m_datas[i]->GetPath());
	}
	m_backHandle = assetManager.GetImageHandle(kMissionBoardPath.c_str());
	m_backShadowHandle = assetManager.GetImageHandle(kMissionBoardShadowPath.c_str());
	m_clearMarkHandle = assetManager.GetImageHandle(kMissionBoardClearPath.c_str());
	m_scoreBoardHandle = assetManager.GetImageHandle(kScoreBoardPath.c_str());
	m_scoreBoardShadowHandle = assetManager.GetImageHandle(kScoreBoardShadowPath.c_str());

	//フォント
	m_textFontHandle = assetManager.GetFontHandle(AssetManager::Font(AssetManager::FontType::NotoSansJP, AssetManager::FontSize::Size20));
	m_titleFontHandle = assetManager.GetFontHandle(AssetManager::Font(AssetManager::FontType::NotoSansJP, AssetManager::FontSize::Size32));
	m_scoreFontHandle = assetManager.GetFontHandle(AssetManager::Font(AssetManager::FontType::NotoSansJP, AssetManager::FontSize::Size12));
}

MissionBoardUI::~MissionBoardUI()
{
}

void MissionBoardUI::Update()
{
}

void MissionBoardUI::Draw() const
{
	if (!m_isDraw)return;
	
	//影描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRotaGraph(kMissionBoardPosX + kMissionBoardShadowOffsetX, kMissionBoardPosY + kMissionBoardShadowOffsetY, 1.0f, 0.0f, m_backShadowHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//背景描画
	DrawRotaGraph(kMissionBoardPosX, kMissionBoardPosY, 1.0f, 0.0f, m_backHandle, true);

	//ミッションボード描画
	DrawRotaGraph(kMissionBoardPosX, kMissionBoardPosY, 1.0f, 0.0f, m_handles.at(m_index), true);

	//テキスト
	auto text = m_datas[m_index]->GetText();
	DrawStringFToHandle(kMissionBoardTextPosX, kMissionBoardTextPosY, text.c_str(), 0x000000, m_textFontHandle);

	//タイトル
	auto title = m_datas[m_index]->GetTitle();
	//中央ぞろえ
	float titleWidth = static_cast<float>(GetDrawNStringWidthToHandle(title.c_str(), wcslen(title.c_str()), m_titleFontHandle));
	if (titleWidth > 0.0f)
	{
		titleWidth *= 0.5f;
	}
	DrawStringFToHandle(kMissionBoardTitlePosX - titleWidth,kMissionBoardTitlePosY, title.c_str(), 0x000000, m_titleFontHandle);

	//クリアしたか
	auto clearData = SaveDataManager::GetInstance().GetClearData();
	if (m_index == 0 && clearData->IsClearStage1() ||
		m_index == 1 && clearData->IsClearStage2() ||
		m_index == 2 && clearData->IsClearStage3())
	{
		DrawRotaGraph(kMissionBoardPosX, kMissionBoardPosY, 1.0f, 0.0f, m_clearMarkHandle, true);
	}

	
	//影描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRotaGraph(kScoreBoardPosX + kScoreBoardPosShadowOffsetX, kScoreBoardPosY + kScoreBoardPosShadowOffsetY, 1.0f, 0.0f, m_scoreBoardShadowHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ハイスコアを描画
	DrawRotaGraph(kScoreBoardPosX, kScoreBoardPosY, 1.0f, 0.0f, m_scoreBoardHandle, true);

	//中央ぞろえ
	float scoreWidth = static_cast<float>(GetDrawNStringWidthToHandle(title.c_str(), wcslen(title.c_str()), m_scoreFontHandle));
	if (scoreWidth > 0.0f)
	{
		scoreWidth *= 0.5f;
	}
	DrawStringFToHandle(kScoreBoardPosX - scoreWidth, kScoreBoardPosY, title.c_str(), 0x000000, m_scoreFontHandle);

}
