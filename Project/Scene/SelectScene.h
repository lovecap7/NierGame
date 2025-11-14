#pragma once
#include "SceneBase.h"
class SelectScene :
    public SceneBase
{
public:
	SelectScene(SceneController& controller);
	~SelectScene();
	/// <summary>
	/// 派生クラスで実装を実装
	/// virtual はこれを継承するかもしれないから
	/// overrideは明示的にオーバーライドをエディタに示すため
	/// </summary>
	virtual void Init()override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void End() override;
	virtual void Restart() override {};
	//デバッグ用
	virtual void DebugDraw() const override;
private:
	enum class Menu : int
	{
		Tutorial1 = 0,
		Tutorial2 = 1,
		Tutorial3 = 2,
		Stage1 = 3,
		Stage2 = 4,
		Title = 5,
	};
	Menu m_currentMenu = Menu::Tutorial1;

};

