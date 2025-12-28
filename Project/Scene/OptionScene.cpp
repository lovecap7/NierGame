#include "OptionScene.h"
#include "SceneController.h"
#include "../Game/UI/Option/OptionUI.h"
#include "../General/Input.h"

OptionScene::OptionScene(SceneController& controller):
	SceneBase(controller),
	m_optionUI(),
	m_currentOptionMenu(OptionMenu::BGM)
{
}

OptionScene::~OptionScene()
{
	if (m_optionUI.expired())return;
	m_optionUI.lock()->Delete();
}

void OptionScene::Init()
{
	//オプションUI作成
	auto optionUI = std::make_shared<OptionUI>();
	optionUI->Init();
	m_optionUI = optionUI;
}

void OptionScene::Update()
{
	if(Input::GetInstance().IsTrigger("B"))
	{
		//メインメニューへ戻る
		m_controller.PopScene();
		return;
	}

	//メニュー操作
	int menu = static_cast<int>(m_currentOptionMenu);
	auto& input = Input::GetInstance();
	if (input.IsRepeate("Up"))menu--;
	if (input.IsRepeate("Down"))menu++;
	//範囲外ならループ
	if (menu < static_cast<int>(OptionMenu::BGM))menu = static_cast<int>(OptionMenu::Max) - 1;
	if (menu >= static_cast<int>(OptionMenu::Max))menu = static_cast<int>(OptionMenu::BGM);
	m_currentOptionMenu = static_cast<OptionMenu>(menu);

	if (m_optionUI.expired())return;
	m_optionUI.lock()->SetSelectMenuIndex(m_currentOptionMenu);
}

void OptionScene::Draw()
{
}

void OptionScene::End()
{
}
