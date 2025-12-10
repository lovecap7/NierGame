#pragma once
#include "SoundBase.h"
class BGM :
    public SoundBase
{
public:
	BGM(int handle, int volume);
	~BGM();
	void Update()override;
	void ChangeBGM(int handle);
};

