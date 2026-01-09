#include "Voice.h"
#include <DxLib.h>

namespace
{
	constexpr int kAdjustmentValue = 50;
}


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

void Voice::SetVolume(SoundManager::SoundVolume volume)
{
	m_volume = volume;
	ChangeVolumeSoundMem(static_cast<int>(m_volume) + kAdjustmentValue, m_playHandle);
}
