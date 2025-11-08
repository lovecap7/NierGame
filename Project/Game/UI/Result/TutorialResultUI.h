#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
#include "../../../General/StageIndex.h"    
#include <memory>
#include <string>
class Timer;
class TutorialResultUI :
    public UIBase
{
public:
    TutorialResultUI(StageIndex tutorialIndex);
    ~TutorialResultUI();
    //更新
    void Update() override;
    //描画
    void Draw()const override;
    //動きが終了したか
    bool IsFinish()const { return m_isFinish; };
private:
    //ハンドル
    int m_mcHandle;
    //背景
    int m_backHandle;
private:
    //フレーム
    int m_countFrame;
    //動きが終了したか
    bool m_isFinish;
    //座標
    Vector2 m_pos;
};

