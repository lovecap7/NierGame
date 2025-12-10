#include "SE.h"
#include "SoundManager.h"

SE::SE(int handle, int volume,bool isLoop) :
	SoundBase(handle, volume, isLoop)
{
}

SE::~SE()
{
}
void SE::Update()
{
	SetVolume(SoundManager::GetInstance().GetSEVolumeC());
}