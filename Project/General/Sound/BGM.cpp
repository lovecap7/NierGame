#include "BGM.h"
#include <DxLib.h>


BGM::BGM(int handle, SoundManager::SoundVolume volume):
	SoundBase(handle,volume,true)
{
}

BGM::~BGM()
{
}
void BGM::Update()
{
	SetVolume(SoundManager::GetInstance().GetBGMVolume());
}

void BGM::ChangeBGM(int handle)
{
	//Ä¶’†‚Ìƒnƒ“ƒhƒ‹‚ª‚ ‚é‚È‚ç
	if (m_playHandle >= 0)
	{
		//Ä¶‚ğ~‚ß‚é
		Stop();
		//íœ
		DeleteSoundMem(m_playHandle);
	}
	//Ä¶
	Play();
}
