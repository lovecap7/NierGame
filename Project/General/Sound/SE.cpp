#include "SE.h"

SE::SE(int handle, SoundManager::SoundVolume volume,bool isLoop) :
	SoundBase(handle, volume, isLoop)
{
}

SE::~SE()
{
}
void SE::Update()
{
	SetVolume(SoundManager::GetInstance().GetSEVolume());
}