#pragma once
#include "SceneBase.h"
#include "../General/Math/MyMath.h"
#include <memory>
#include <map>
class SceneController;
class OptionUI;
class OptionScene :
    public SceneBase
{
public:
    OptionScene(SceneController& controller);
    ~OptionScene();
    //派生クラスで実装を実装
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
    virtual void DebugDraw()const override {};

	enum class OptionMenu : int
    {
        BGM = 0,
		SE = 1,
		Voice = 2,
        ScreenMode = 3,
		Max = 4,
	};
private:
	//オプションUI
	std::weak_ptr<OptionUI> m_optionUI;

	//オプションメニューの選択中インデックス
	OptionMenu m_currentOptionMenu;
};

