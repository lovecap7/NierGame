#include "SoundBase.h"
#include "SoundManager.h"
#include <DxLib.h>

SoundBase::SoundBase(int handle, int volume, bool isLoop):
	m_playHandle(handle),
	m_volume(volume),
	m_isLoop(isLoop)
{
	SetVolume(m_volume);
}

SoundBase::~SoundBase()
{
}

void SoundBase::Init()
{
	SoundManager::GetInstance().Entry(shared_from_this());
}
void SoundBase::End()
{
	Delete();
	SoundManager::GetInstance().Exit(shared_from_this());
}
void SoundBase::Play()
{
	SetVolume(m_volume);
	//再生
	if (m_isLoop)	
	{
		//ループ
		PlaySoundMem(m_playHandle, DX_PLAYTYPE_LOOP, true);
	}
	else
	{
		PlaySoundMem(m_playHandle, DX_PLAYTYPE_BACK, true);
	}
}
void SoundBase::Stop()
{
	//再生を止める
	StopSoundMem(m_playHandle);
}
void SoundBase::Delete()
{
	//削除
	DeleteSoundMem(m_playHandle);
}

bool SoundBase::IsPlay()
{
	return CheckSoundMem(m_playHandle);
}

bool SoundBase::IsDelete()
{
	//ループ再生じゃなく再生が終了したとき削除する
	return !m_isLoop && IsPlay();
}

void SoundBase::SetVolume(int volume)
{
	m_volume = volume;
	ChangeVolumeSoundMem(m_volume, m_playHandle);
}
