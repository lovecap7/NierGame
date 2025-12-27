#pragma once
#include "../UIBase.h"
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
private:
	//ハンドル
	int m_bgmHandle;
	int m_seHandle;
	int m_voiceHandle;
	int m_screenModeHandle;
	int m_windowHandle;
	int m_fullScreenHandle;
	int m_cursorHandle;
};

