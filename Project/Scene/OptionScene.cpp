#include "OptionScene.h"
#include "SceneController.h"
#include "../Game/UI/Option/OptionUI.h"
#include "../General/Input.h"
#include "../General/Sound/SoundManager.h"

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
	auto& input = Input::GetInstance();

	//音量調整
	auto& soundManager = SoundManager::GetInstance();
	switch (m_currentOptionMenu)
	{
	case OptionScene::OptionMenu::BGM:
		if (input.IsRepeate("Right"))soundManager.LevelUpBGMVolume();
		if (input.IsRepeate("Left"))soundManager.LevelDownBGMVolume();
		break;
	case OptionScene::OptionMenu::SE:
		if (input.IsRepeate("Right"))soundManager.LevelUpSEVolume();
		if (input.IsRepeate("Left"))soundManager.LevelDownSEVolume();
		break;
	case OptionScene::OptionMenu::Voice:
		if (input.IsRepeate("Right"))soundManager.LevelUpVoiceVolume();
		if (input.IsRepeate("Left"))soundManager.LevelDownVoiceVolume();
		break;
	case OptionScene::OptionMenu::ScreenMode:
		break;
	case OptionScene::OptionMenu::Max:
		break;
	default:
		break;
	}


	//メニュー操作
	int menu = static_cast<int>(m_currentOptionMenu);
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
