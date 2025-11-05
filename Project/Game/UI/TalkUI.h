#pragma once
#include "UIBase.h"
class TalkUI :
    public UIBase
{
private:
    TalkUI();
    ~TalkUI();
    //XV
    void Update() override;
    //•`‰æ
    void Draw()const override;
};

