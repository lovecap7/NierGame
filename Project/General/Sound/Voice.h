#pragma once
#include "SoundBase.h"
class Voice :
    public SoundBase
{
public:
    Voice(int handle, int volume);
    ~Voice();
    void Update()override;
};

