#include "Voice.h"


Voice::Voice(int handle, SoundManager::SoundVolume volume):
	SoundBase(handle,volume,false)
{
}

Voice::~Voice()
{
}
void Voice::Update()
{
	SetVolume(SoundManager::GetInstance().GetVoiceVolume());
}