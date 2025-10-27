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
	//フェードイン
	void FadeIn(float speed = 5.0f);
	//フェードアウト
	void FadeOut(float speed = 5.0f);
	//フェードインが終了したか
	bool IsFinishFadeIn()const;
	//フェードアウトが終了したか
	bool IsFinishFadeOut()const;
	//フェード中か
	bool IsFadeNow()const { return m_isFadeNow; };
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

