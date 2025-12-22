#pragma once
#include <map>
#include <string>
#include <list>
#include <memory>
class SoundBase;
class BGM;
class SE;
class Voice;
/// <summary>
/// 音(BGM,SE)を管理するシングルトンクラス
/// </summary>
class SoundManager final
{
private:
	//シングルトンの準備
	SoundManager() = default;
	~SoundManager() = default;
	//コピー禁止
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator = (const SoundManager&) = delete;
	//ムーブ禁止
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator = (SoundManager&&) = delete;
public:
	//インスタンスを取得
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}
	//登録
	void Entry(std::shared_ptr<SoundBase> sound);
	//登録解除
	void Exit(std::shared_ptr<SoundBase> sound);
	//初期化
	void Init();
	//更新
	void Update();
	//終了
	void End();
	//保存
	void SaveVolume();
	//BGM再生
	void PlayBGM(std::wstring path);
	void PlayBGM();
	//BGM停止
	void StopBGM();
	//1度だけ再生する 
	std::weak_ptr<SE> PlayOnceSE(std::wstring path);
	//ループ再生する
	std::weak_ptr<SE> PlayLoopSE(std::wstring path);
	//1度だけ再生する 
	std::weak_ptr<Voice> PlayVC(std::wstring path);
	//全ての再生を開始
	void AllPlay();
	//全ての再生を止める
	void AllStop();
	//BGMのロード
	int LoadBGM(std::wstring path);
	//SEのロード
	int LoadSE(std::wstring path);
	//VCのロード
	int LoadVC(std::wstring path);
	//音量
	int GetSEVolumeC()	 const;
	int GetBGMVolumeC()	 const;
	int GetVoiceVolumeC() const;
	int GetMasterVolume()const;
	//セッター
	void SetSEVolume(int volume);
	void SetBGMVolume(int volume);
	void SetVoiceVolume(int volume);
	void SetMasterVolume(int volume);
	//音量調整用
	int GetSEVolume()	 const { return m_seVolume; };
	int GetBGMVolume()	 const { return m_bgmVolume; };
	int GetVoiceVolume() const { return m_voiceVolume; };
private:
	//SEの音量
	int m_seVolume;
	//BGMの音量
	int m_bgmVolume;
	//ボイスの音量
	int m_voiceVolume;
	//マスターの音量
	int m_masterVolume;
	//サウンド
	std::list<std::shared_ptr<SoundBase>> m_sounds;
	//BGMは常に一つなのでここで扱う
	std::shared_ptr<BGM> m_bgm;
	//削除候補のものを削除
	void CheckDeleteSound();
};
