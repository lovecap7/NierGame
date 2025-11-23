#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
#include "../../../General/StageIndex.h"    
#include <memory>
#include <string>
class Timer;
class GameoverUI :
    public UIBase
{
public:
    GameoverUI();
    ~GameoverUI();
    //更新
    void Update() override;
    //描画
    void Draw()const override;
  
private:
    //背景
    int m_backHandle;
	//ゲームオーバー文字
	int m_gameoverHandle;
    //続きから
	int m_continueHandle;
    //はじめから
	int m_restartHandle;
	//ステージセレクトへ
	int m_stageSelectHandle;
private:
    //フレームカウント
	int m_countFrame;
    //ゲームオーバー文字の位置
	Vector2 m_gameoverPos;
    //メニューの位置
    Vector2 m_menuPos;
    //メニューインデックス
    enum class Menu : int
    {
        Continue = 0,
        Restart = 1,
        StageSelect = 2,
    };
	Menu m_menuIndex;
};

