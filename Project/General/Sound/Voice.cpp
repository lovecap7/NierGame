#include "Voice.h"
#include "SoundManager.h"

Voice::Voice(int handle, int volume):
	SoundBase(handle,volume,false)
{
}

Voice::~Voice()
{
}
void Voice::Update()
{
	SetVolume(SoundManager::GetInstance().GetVoiceVolumeC());
}