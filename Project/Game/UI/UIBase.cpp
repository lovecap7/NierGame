#include "UIBase.h"
#include "UIManager.h"

UIBase::UIBase():
	m_isFrontDraw(false),
	m_isDelete(false)
{
}

UIBase::~UIBase()
{
}

void UIBase::Init()
{
	//UIManager‚É“o˜^
	UIManager::GetInstance().Entry(shared_from_this());
}

void UIBase::End()
{
	//UIManager‚©‚ç“o˜^‰ğœ
	UIManager::GetInstance().Exit(shared_from_this());
}