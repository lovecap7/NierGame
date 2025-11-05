#pragma once
#include <memory>
class UIBase abstract:
	public std::enable_shared_from_this<UIBase>
{
public:
	UIBase();
	~UIBase();
	//‰Šú‰»
	virtual void Init();
	//I—¹
	virtual void End();
	//XV
	virtual void Update() abstract;
	//•`‰æ
	virtual void Draw()const abstract;
	//•`‰æ‡
	bool IsFrontDraw()const { return m_isFrontDraw; };
	//íœ
	bool IsDelete()const { return m_isDelete; };
protected:
	//•`‰æ‡
	bool m_isFrontDraw;
	//íœ
	bool m_isDelete;
};

