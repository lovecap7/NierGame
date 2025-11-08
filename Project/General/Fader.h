#pragma once
//フェードインとフェードアウトをするクラス
class Fader final
{
private:
	//シングルトンの準備
	Fader() = default;
	~Fader() = default;
	//コピー禁止
	Fader(const Fader&) = delete;
	Fader& operator = (const Fader&) = delete;
	//ムーブ禁止
	Fader(Fader&&) = delete;
	Fader& operator = (Fader&&) = delete;
public:
	/// <summary>
	/// シングルトンインスタンスを取得する
	/// </summary>
	/// <returns></returns>
	static Fader& GetInstance()
	{
		static Fader instance;
		return instance;
	}
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw()const;
	//だんだん明るくなる
	void FadeIn(float speed = 10.0f);
	//だんだん暗くなる
	void FadeOut(float speed = 10.0f);
	//フェードインが終了したか
	bool IsFinishFadeIn()const;
	//フェードアウトが終了したか
	bool IsFinishFadeOut()const;
	//フェード中か
	bool IsFadeNow()const { return m_isFadeNow; };
	bool IsFadeInNow()const { return m_isFadeIn; };
	bool IsFadeOutNow()const { return m_isFadeOut; };
private:
	//だんだん明るくなる
	bool m_isFadeIn;
	//だんだん暗くなる
	bool m_isFadeOut;
	//フレームカウント
	float m_countFrame;
	//フェード速度
	float m_fadeSpeed;
	//フェード中
	bool m_isFadeNow;
};

