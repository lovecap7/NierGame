#include "SoundManager.h"
#include "SoundBase.h"
#include "BGM.h"
#include "SE.h"
#include "Voice.h"
#include "../Math/MyMath.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//最小
	constexpr int kMinVolume = 0;
	//最大
	constexpr int kMaxVolume = 255;
	//デフォルトのサウンドの倍率
	constexpr int kDefaultVolume = 127;
	//補正倍率
	constexpr float kCorrectionRate = 1.2f;
}

void SoundManager::Entry(std::shared_ptr<SoundBase> sound)
{
	//すでに登録されているならしない
	auto it = std::find(m_sounds.begin(), m_sounds.end(), sound);
	if (it != m_sounds.end())return;
	//登録
	m_sounds.emplace_back(sound);
}

void SoundManager::Exit(std::shared_ptr<SoundBase> sound)
{
	//登録されていないならしない
	auto it = std::find(m_sounds.begin(), m_sounds.end(), sound);
	if (it == m_sounds.end())return;
	//解除
	m_sounds.remove(sound);
}

void SoundManager::Init()
{
	
}
void SoundManager::Update()
{
	if (m_bgm)
	{
		m_bgm->Update();
	}
	for (auto& sound : m_sounds)
	{
		sound->Update();
	}
	//再生が終了したハンドルを消す
	CheckDeleteSound();
}

void SoundManager::End()
{
	//削除
	for (auto& sound : m_sounds) {
		sound->Delete();
	}
	m_sounds.clear();
	InitSoundMem();
	SaveVolume();
}

void SoundManager::SaveVolume()
{
	////音量保存
	//auto csvLoader = std::make_shared<CSVDataSaver>();
	//VolumeData volumeData;
	//volumeData.seVolume = m_seVolume / kCorrectionRate;
	//volumeData.bgmVolume = m_bgmVolume;
	//volumeData.voiceVolume = m_voiceVolume / kCorrectionRate;
	//volumeData.masterVolume = m_masterVolume;
	//csvLoader->SaveVolumeDataToCSV(volumeData);
}


void SoundManager::PlayBGM(std::string name)
{
	////すでに実体があるとき終了処理をする
	//if (m_bgm)
	//{
	//	m_bgm->End();
	//}
	//m_bgm = std::make_shared<BGM>(DuplicateSoundMem(m_soundHandles[name]),m_bgmVolume);
	//m_bgm->Init();
	//m_bgm->Play();
}

void SoundManager::StopBGM()
{
	m_bgm->Stop();
}

std::weak_ptr<SE> SoundManager::PlayOnceSE(std::string name)
{
	std::shared_ptr<SE> se;
	//auto it = m_soundHandles.find(name);
	//if (it != m_soundHandles.end()) {
	//	// キーがある場合
	//	se = std::make_shared<SE>(DuplicateSoundMem(m_soundHandles[name]), m_seVolume, false);
	//	se->Init();
	//	se->Play();
	//}
	return se;
}

std::weak_ptr<SE> SoundManager::PlayOnceSE(int handle)
{
	auto se = std::make_shared<SE>(DuplicateSoundMem(handle), m_seVolume, false);
	se->Init();
	se->Play();
	return se;
}

std::weak_ptr<SE> SoundManager::PlayLoopSE(std::string name)
{
	std::shared_ptr<SE> se;
	//auto it = m_soundHandles.find(name);
	//if (it != m_soundHandles.end()) {
	//	// キーがある場合
	//	se = std::make_shared<SE>(DuplicateSoundMem(m_soundHandles[name]), m_seVolume, true);
	//	se->Init();
	//	se->Play();
	//}
	return se;
}

std::weak_ptr<SE> SoundManager::PlayLoopSE(int handle)
{
	auto se = std::make_shared<SE>(DuplicateSoundMem(handle), m_seVolume, true);
	se->Init();
	se->Play();
	return se;
}

std::weak_ptr<Voice> SoundManager::PlayVC(std::string name)
{
	std::shared_ptr<Voice> vc;
	//auto it = m_soundHandles.find(name);
	//if (it != m_soundHandles.end()) {
	//	// キーがある場合
	//	vc = std::make_shared<Voice>(DuplicateSoundMem(m_soundHandles[name]), m_seVolume);
	//	vc->Init();
	//	vc->Play();
	//}
	return vc;
}

std::weak_ptr<Voice> SoundManager::PlayVC(int handle)
{
	auto vc = std::make_shared<Voice>(DuplicateSoundMem(handle), m_seVolume);
	vc->Init();
	vc->Play();
	return vc;
}

void SoundManager::AllPlay()
{
	for (auto& sound : m_sounds)
	{
		sound->Play();
	}
	if (m_bgm)
	{
		m_bgm->Play();
	}
}

void SoundManager::AllStop()
{
	for (auto& sound : m_sounds)
	{
		sound->Stop();
	}
	if (m_bgm)
	{
		m_bgm->Stop();
	}
}

int SoundManager::GetSEVolumeC() const
{
	return MathSub::ClampInt(m_seVolume, kMinVolume, m_masterVolume);
}

int SoundManager::GetBGMVolumeC() const
{
	return MathSub::ClampInt(m_bgmVolume, kMinVolume, m_masterVolume);
}

int SoundManager::GetVoiceVolumeC() const
{
	return MathSub::ClampInt(m_voiceVolume, kMinVolume, m_masterVolume);
}

int SoundManager::GetMasterVolume() const
{
	return m_masterVolume;
}

void SoundManager::SetSEVolume(int volume)
{
	m_seVolume = MathSub::ClampInt(volume, kMinVolume, kMaxVolume);
}

void SoundManager::SetBGMVolume(int volume)
{
	m_bgmVolume = MathSub::ClampInt(volume, kMinVolume, kMaxVolume);
}

void SoundManager::SetVoiceVolume(int volume)
{
	m_voiceVolume = MathSub::ClampInt(volume, kMinVolume, kMaxVolume);
}

void SoundManager::SetMasterVolume(int volume)
{
	m_masterVolume = MathSub::ClampInt(volume, kMinVolume, kMaxVolume);
}

//消滅フラグをチェックして削除
void SoundManager::CheckDeleteSound()
{
	std::list<std::shared_ptr<SoundBase>> deleteSound;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		for (auto& sound : m_sounds)
		{
			if (sound->IsDelete())
			{
				isOneMore = true;
				//削除候補
				deleteSound.emplace_back(sound);
			}
		}
		//削除
		for (auto& sound : deleteSound)
		{
			Exit(sound);
		}
		deleteSound.clear();
		if (!isOneMore)break;
	}
}