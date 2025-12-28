#include "SoundManager.h"
#include "SoundBase.h"
#include "BGM.h"
#include "SE.h"
#include "Voice.h"
#include "../AssetManager.h"
#include "../Math/MyMath.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	//パス
	const std::wstring kBGM = L"BGM/";
	const std::wstring kSE = L"SE/";
	const std::wstring kVoice = L"Voice/";
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
	//SEの音量
	m_seVolume = SoundVolume::V5;
	//BGMの音量
	m_bgmVolume = SoundVolume::V5;
	//ボイスの音量
	m_voiceVolume = SoundVolume::V5;
	//マスターの音量
	m_masterVolume = SoundVolume::V5;
}
void SoundManager::Update()
{
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
	CheckDeleteSound();
	m_sounds.clear();
	InitSoundMem();
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


void SoundManager::PlayBGM(std::wstring path)
{
	//すでに実体があるとき終了処理をする
	if (m_bgm)
	{
		m_bgm->End();
	}
	m_bgm = std::make_shared<BGM>(LoadBGM(path),m_bgmVolume);
	m_bgm->Init();
	m_bgm->Play();
}

void SoundManager::PlayBGM()
{
	if (!m_bgm)return;
	m_bgm->Play();
}

void SoundManager::StopBGM()
{
	m_bgm->Stop();
}

std::weak_ptr<SE> SoundManager::PlayOnceSE(std::wstring path)
{
	std::shared_ptr<SE> se;
	se = std::make_shared<SE>(LoadSE(path), m_seVolume, false);
	se->Init();
	se->Play();
	return se;
}


std::weak_ptr<SE> SoundManager::PlayLoopSE(std::wstring path)
{
	std::shared_ptr<SE> se;
	se = std::make_shared<SE>(LoadSE(path), m_seVolume, true);
	se->Init();
	se->Play();
	return se;
}

std::weak_ptr<Voice> SoundManager::PlayVoice(std::wstring path)
{
	std::shared_ptr<Voice> vc;
	vc = std::make_shared<Voice>(LoadVoice(path), m_seVolume);
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

int SoundManager::LoadBGM(std::wstring path)
{
	return AssetManager::GetInstance().GetSoundHandle(kBGM + path);
}

int SoundManager::LoadSE(std::wstring path)
{
	return AssetManager::GetInstance().GetSoundHandle(kSE + path);
}

int SoundManager::LoadVoice(std::wstring path)
{
	return AssetManager::GetInstance().GetSoundHandle(kVoice + path);
}

void SoundManager::SetSEVolume(SoundVolume volume)
{
	m_seVolume = volume;
}

void SoundManager::SetBGMVolume(SoundVolume volume)
{
	m_bgmVolume = volume;
}

void SoundManager::SetVoiceVolume(SoundVolume volume)
{
	m_voiceVolume = volume;
}

void SoundManager::SetMasterVolume(SoundVolume volume)
{
	m_masterVolume = volume;
}

int SoundManager::GetVolumeLevel(SoundVolume volume) const
{
	switch(volume)
	{
		case SoundVolume::V0:
			return 0;
		case SoundVolume::V1:
			return 1;
		case SoundVolume::V2:
			return 2;
		case SoundVolume::V3:
			return 3;
		case SoundVolume::V4:
			return 4;
		case SoundVolume::V5:
			return 5;
		case SoundVolume::V6:
			return 6;
		case SoundVolume::V7:
			return 7;
		case SoundVolume::V8:
			return 8;
		case SoundVolume::V9:
			return 9;
		case SoundVolume::V10:
			return 10;
	}

	return 0;
}

//消滅フラグをチェックして削除
void SoundManager::CheckDeleteSound()
{
	if (m_sounds.empty())return;

	std::list<std::shared_ptr<SoundBase>> deleteSound;
	for (auto& sound : m_sounds)
	{
		if (sound->IsDelete())
		{
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
}