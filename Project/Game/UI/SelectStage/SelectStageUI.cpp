#include "SelectStageUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Fader.h"
#include <DxLib.h>

namespace
{
	//メインメニューの位置
	constexpr float kMainMenuStartX = 200.0f;
	constexpr float kMainMenuOffsetX = -600.0f;
	constexpr float kMainMenuStartY = 110.0f;
	constexpr float kMainMenuOffsetY = 110.0f;
	constexpr float kMainMenuLerpRate = 0.3f;

	//チュートリアルメニューの位置
	constexpr float kTutorialMenuStartX = 600.0f;
	constexpr float kTutorialMenuStartY = 110.0f;
	constexpr float kTutorialMenuOffsetY = 110.0f;
	constexpr float kTutorialMenuLerpRate = 0.3f;

	//ステージメニューの位置
	constexpr float kStageMenuStartX = 600.0f;
	constexpr float kStageMenuStartY = 110.0f;
	constexpr float kStageMenuOffsetY = 110.0f;
	constexpr float kStageMenuLerpRate = 0.3f;

	//選択中にずらす量
	constexpr float kSelectOffsetX = 30.0f;
}

SelectStageUI::SelectStageUI(int mainSize, int tutorialSize, int stageSelectSize):
	m_testHandle(-1),
	m_selectMainMenuIndex(0),
	m_selectTutorialMenuIndex(0),
	m_selectStageMenuIndex(0),
	m_update(&SelectStageUI::UpdateMainMenu)
{
	m_testHandle = AssetManager::GetInstance().GetImageHandle(L"Select/Test");
	
	//メインメニュー
	m_mainMenuPos.resize(mainSize);
	for(int i = 0; i < mainSize; ++i)
	{
		m_mainMenuPos[i] = Vector2(kMainMenuStartX + kMainMenuOffsetX * (i + 1), kMainMenuStartY + i * kMainMenuOffsetY);
	}

	//チュートリアルメニュー
	m_tutorialMenuPos.resize(tutorialSize);
	for (int i = 0; i < tutorialSize; ++i)
	{
		m_tutorialMenuPos[i] = Vector2(kMainMenuStartX , kMainMenuStartY);
	}

	//ステージメニュー
	m_stageMenuPos.resize(stageSelectSize);
	for (int i = 0; i < stageSelectSize; ++i)
	{
		m_stageMenuPos[i] = Vector2(kMainMenuStartX , kMainMenuStartY + kMainMenuOffsetY);
	}
}

SelectStageUI::~SelectStageUI()
{
}

void SelectStageUI::Update()
{
	(this->*m_update)();
}

void SelectStageUI::Draw() const
{
	auto& fader = Fader::GetInstance();

	//チュートリアルメニュー描画
	DrawMenu(fader, m_tutorialMenuPos, m_selectTutorialMenuIndex,m_update == &SelectStageUI::UpdateTutorialMenu);

	//ステージメニュー描画
	DrawMenu(fader, m_stageMenuPos, m_selectStageMenuIndex, m_update == &SelectStageUI::UpdateStageMenu);

	//メインメニュー描画
	DrawMenu(fader, m_mainMenuPos, m_selectMainMenuIndex, m_update == &SelectStageUI::UpdateMainMenu);
}

void SelectStageUI::SetSelectMainMenuIndex(int index)
{
	m_selectMainMenuIndex = index;
	m_update = &SelectStageUI::UpdateMainMenu;
}

void SelectStageUI::SetSelectTutorialMenuIndex(int index)
{
	m_selectTutorialMenuIndex = index;
	m_update = &SelectStageUI::UpdateTutorialMenu;
}

void SelectStageUI::SetSelectStageMenuIndex(int index)
{
	m_selectStageMenuIndex = index;
	m_update = &SelectStageUI::UpdateStageMenu;
}

void SelectStageUI::UpdateMainMenu()
{
	auto& fader = Fader::GetInstance();
	//メインメニュー
	for (int i = 0; i < m_mainMenuPos.size(); ++i)
	{
		float targetX = kMainMenuStartX;
		//選んでいるメニューなら右に動かす
		if (m_selectMainMenuIndex == i && (!fader.IsFadeNow() || (fader.IsFadeOutNow())))
		{
			targetX += kSelectOffsetX;
		}
		m_mainMenuPos[i].x = MathSub::Lerp(m_mainMenuPos[i].x, targetX, kMainMenuLerpRate);
	}
	//チュートリアル
	for(int i = 0; i < m_tutorialMenuPos.size(); ++i)
	{
		if(m_mainMenuPos.empty())
		{
			break;
		}
		m_tutorialMenuPos[i] = Vector2::Lerp(m_tutorialMenuPos[i], m_mainMenuPos[0], kMainMenuLerpRate);
	}
	//ステージ
	for(int i = 0; i < m_tutorialMenuPos.size(); ++i)
	{
		if(m_mainMenuPos.empty())
		{
			break;
		}
		m_stageMenuPos[i] = Vector2::Lerp(m_stageMenuPos[i], m_mainMenuPos[1], kMainMenuLerpRate);
	}
}

void SelectStageUI::UpdateTutorialMenu()
{
	for (int i = 0; i < m_tutorialMenuPos.size(); ++i)
	{
		float targetX = kTutorialMenuStartX;
		float targetY = kTutorialMenuStartY + kTutorialMenuOffsetY * i;
		//選んでいるメニューなら右に動かす
		if (m_selectTutorialMenuIndex == i)
		{
			targetX += kSelectOffsetX;
		}
		m_tutorialMenuPos[i].x = MathSub::Lerp(m_tutorialMenuPos[i].x, targetX, kTutorialMenuLerpRate);
		m_tutorialMenuPos[i].y = MathSub::Lerp(m_tutorialMenuPos[i].y, targetY, kTutorialMenuLerpRate);
	}
}

void SelectStageUI::UpdateStageMenu()
{
	for (int i = 0; i < m_stageMenuPos.size(); ++i)
	{
		float targetX = kStageMenuStartX;
		float targetY = kStageMenuStartY + kStageMenuOffsetY * i;
		//選んでいるメニューなら右に動かす
		if (m_selectStageMenuIndex == i)
		{
			targetX += kSelectOffsetX;
		}
		m_stageMenuPos[i].x = MathSub::Lerp(m_stageMenuPos[i].x, targetX, kStageMenuLerpRate);
		m_stageMenuPos[i].y = MathSub::Lerp(m_stageMenuPos[i].y, targetY, kStageMenuLerpRate);
	}
}

void SelectStageUI::DrawMenu(Fader& fader, const std::vector<Vector2>& menuPos,int selectMenuIndex, bool isMenuMode) const
{
	for (int i = 0; i < menuPos.size(); ++i)
	{
		//選んでいるメニューなら反転
		if (selectMenuIndex == i && 
			(!fader.IsFadeNow() || (fader.IsFadeOutNow())) &&
			isMenuMode)
		{
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
		}
		DrawRotaGraph(menuPos[i].x, menuPos[i].y, 1.0, 0.0, m_testHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

