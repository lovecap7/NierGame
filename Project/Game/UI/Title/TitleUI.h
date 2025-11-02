#pragma once
#include "../UIBase.h"
class TitleUI :
    public UIBase
{
public:
    TitleUI();
    ~TitleUI();
    //XV
    void Update() override;
    //•`‰æ
    void Draw()const override;
private:
    //ƒ^ƒCƒgƒ‹ƒƒS
    int m_titleLogoHandle;
    //”wŒi
    int m_titleBackHandle;
};

