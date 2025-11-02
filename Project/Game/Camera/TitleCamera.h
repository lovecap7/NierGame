#pragma once
#include "CameraBase.h"
class TitleCamera :
    public CameraBase
{
public:
    TitleCamera();
    ~TitleCamera();
    virtual void Init() override;
    virtual void Update() override;
};

