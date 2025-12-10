#pragma once
#include "SoundBase.h"
class SE :
    public SoundBase
{
public:
    SE(int handle, int volume, bool isLoop);
    ~SE();
    void Update()override;
};

