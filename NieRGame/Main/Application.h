#pragma once
#include<memory>
class ShaderPostProcess;
/// <summary>
/// シングルトン
/// </summary>
class Application final
{
private:
	//シングルトンの準備
	Application() = default;
	~Application() = default;
	//コピー禁止
	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;
	//ムーブ禁止
	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;
public:
	/// <summary>
	/// シングルトンオブジェクトを返す関数
	/// 参照を返すことで、複製を防ぐ
	/// 間違えて値を返した場合、コピーコンストラクタが走るので
	/// それをprivateにしておくことで弾くことができる
	/// </summary>
	/// <returns></returns>
	static Application& GetInstance();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>ture:初期化成功, false:失敗</returns>
	bool Init();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Run();
	/// <summary>
	/// 終了処理
	/// </summary>
	void Terminate();

	//アプリケーションを終了させる
	void FinishApplication() { m_isFinishApplication = true; };
	//Window切り替え
	void SetWindowMode(bool isWindow);
	bool IsWindowMode()const { return m_isWindow; };
	//デルタタイムを取得
	float GetDeltaTime() const { return m_deltaTime; };
	//タイムスケールを設定
	void SetTimeScale(float timeScale) { m_timeScale = timeScale; };
	//タイムスケールを取得
	float GetTimeScale() const { return m_timeScale; };
	//タイムスケールを考慮したデルタタイムを取得
	float GetDeltaTimeWithTimeScale() const { return m_deltaTime * m_timeScale; };
private:
	//アプリケーションの終了
	bool m_isFinishApplication;
	//ウィンドウモードにする
	bool m_isWindow = true;
	//デルタタイム
	float m_deltaTime;
	//タイムスケール
	float m_timeScale;
	//ポストエフェクト
	std::unique_ptr<ShaderPostProcess> m_postProcess;
private:
	//スクリーンモードを切り替える際の処理
	void ChangeScreenMode();
	//ターゲットFPS
	void DebugDrawFPS(float totalTime, int targetFPS)const;
};
