#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
#include <vector>
class Fader;
class SelectStageUI :
    public UIBase
{
public:
	SelectStageUI(int mainSize,int tutorialSize,int stageSelectSize);
	~SelectStageUI();
	//更新
	void Update() override;
	//描画
	void Draw()const override;
	void DrawMenu(Fader& fader) const;
	//選んでるメニューのインデックスを設定
	void SetSelectMainMenuIndex(int index);
	void SetSelectTutorialMenuIndex(int index);
	void SetSelectStageMenuIndex(int index);
private:
	//テスト
	int m_testHandle;

	//メインメニューの項目座標
	std::vector<Vector2> m_mainMenuPos;
	//選んでるメニューのインデックス
	int m_selectMainMenuIndex;

	//チュートリアルメニューの項目座標
	std::vector<Vector2> m_tutorialMenuPos;
	//選んでるメニューのインデックス
	int m_selectTutorialMenuIndex;

	//ステージメニューの項目座標
	std::vector<Vector2> m_stageMenuPos;
	//選んでるメニューのインデックス
	int m_selectStageMenuIndex;

	//状態に合わせた更新
	using UpdateFunc = void(SelectStageUI::*)();
	UpdateFunc m_update;

	//メインメニューの更新
	void UpdateMainMenu();
	//チュートリアルメニューの更新
	void UpdateTutorialMenu();
	//ステージメニューの更新
	void UpdateStageMenu();

	//メニュー描画
	void DrawMenu(Fader& fader,const std::vector<Vector2>& menuPos, int selectMeueIndex,bool isMenuMode) const;
};

