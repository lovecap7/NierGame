#pragma once
#include "../UIBase.h"
#include "../../../Scene/OptionScene.h"
#include <vector>
class OptionUI :
    public UIBase
{
public:
	OptionUI();
	~OptionUI();
	//更新
	void Update() override;
	//描画
	void Draw() const override;

	//選択中メニューのインデックスを設定
	void SetSelectMenuIndex(int index) { m_selectMenuIndex = index; };
private:
	//ハンドル
	int m_bgmHandle;
	int m_seHandle;
	int m_voiceHandle;
	int m_screenModeHandle;
	int m_windowHandle;
	int m_fullScreenHandle;
	int m_cursorHandle;

	//選択中メニュー
	int m_selectMenuIndex;

	//座標
	std::vector<Vector2> m_menuPos;
};

