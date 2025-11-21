#pragma once
#include "../SceneBase.h"
#include "../../General/StageIndex.h"
class DebugSelectScene :
    public SceneBase
{
public:
	DebugSelectScene(SceneController& controller);
	~DebugSelectScene();
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
		Title		= 0,
		Stage1		= 1,
		Stage2		= 2,
		Stage3		= 3,
		Result		= 4,
		Tutorial1	= 5,
		Tutorial2	= 6,
		Tutorial3	= 7,
		PlayerTest	= 8,
	};
	Menu m_currentMenu = Menu::Title;
};

