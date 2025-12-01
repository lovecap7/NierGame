#pragma once
#include "../UIBase.h"
#include <memory>
class ShaderPostProcess;
class TitleUI :
    public UIBase
{
public:
    TitleUI();
    ~TitleUI();
    //更新
    void Update() override;
    //描画
    void Draw()const override;
private:
    //タイトルロゴ
    int m_titleLogoHandle;
    //背景
    int m_titleBackHandle;
    //シェーダ
	std::shared_ptr<ShaderPostProcess> m_shader;
    //不定期に揺れる
};

