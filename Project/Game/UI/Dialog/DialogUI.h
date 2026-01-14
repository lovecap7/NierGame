#pragma once
#include "../UIBase.h"
class DialogUI :
    public UIBase
{
public:
    DialogUI();
    ~DialogUI();
    void Update()override;
    void Draw()const override;
private:
    //ƒnƒ“ƒhƒ‹
    int m_yesHandle;
    int m_noHandle;
    int m_backHandle;

};

