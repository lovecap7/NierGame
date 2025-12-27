#include "OptionScene.h"
#include "../Game/UI/Option/OptionUI.h"

OptionScene::OptionScene(SceneController& controller):
	SceneBase(controller),
	m_optionUI()
{
}

OptionScene::~OptionScene()
{
	if (m_optionUI.expired())return;
	m_optionUI.lock()->Delete();
}

void OptionScene::Init()
{
	//ƒIƒvƒVƒ‡ƒ“UIì¬
	auto optionUI = std::make_shared<OptionUI>();
	optionUI->Init();
	m_optionUI = optionUI;
}

void OptionScene::Update()
{
}

void OptionScene::Draw()
{
}

void OptionScene::End()
{
}
