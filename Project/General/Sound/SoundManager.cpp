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
	//最小
	constexpr int kMinVolume = 0;
	//最大
	constexpr int kMaxVolume = 255;
	//デフォルトのサウンドの倍率
	constexpr int kDefaultVolume = 127;
	//補正倍率
	constexpr float kCorrectionRate = 1.2f;

	//パス
	const std::wstring kBGM = L"BGM/";
	const std::wstring kSE = L"SE/";
	const std::wstring kVC = L"VC/";
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
	m_seVolume = kDefaultVolume;
	//BGMの音量
	m_bgmVolume = kDefaultVolume;
	//ボイスの音量
	m_voiceVolume = kDefaultVolume;
	//マスターの音量
	m_masterVolume = kDefaultVolume;
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

std::weak_ptr<Voice> SoundManager::PlayVC(std::wstring path)
{
	std::shared_ptr<Voice> vc;
	vc = std::make_shared<Voice>(LoadVC(path), m_seVolume);
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

int SoundManager::LoadVC(std::wstring path)
{
	return AssetManager::GetInstance().GetSoundHandle(kVC + path);
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