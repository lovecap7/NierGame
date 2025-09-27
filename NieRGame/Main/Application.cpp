#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include  <cassert>
namespace
{
	//ターゲットFPS
	const int kTargetFPS = 120;
	//ミリ秒
	float kMillisecond = 1000.0f;
	//タイムスケール
	float kTimeScale = 1.0f;
}

Application& Application::GetInstance()
{
    //この時点でメモリが確保されてアプリ終了まで残る
    static Application app;
    return app;
}

bool Application::Init()
{
	//初期化
	//このフラグがtrueの時アプリケーションが終了する
	m_isFinishApplication = false;

	//ゲームタイトル
	SetWindowText(L"NieR");
	//ゲームアイコン
	//SetWindowIconID(IDI_ICON1);

	 //フルスクリーンでなく、ウィンドウモードで開くようにする
	//こういった関数はウィンドウが開く前に(Dxlib.Init()の前)に処理しておく必要がある
	ChangeWindowMode(m_isWindow);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//画面サイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return false;			// エラーが起きたら直ちに終了
	}

	//描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//3D描画のための準備
	//Zバッファを使用する
	SetUseZBuffer3D(true);
	//Zバッファに書き込みを行う
	SetWriteZBuffer3D(true);
	//バックカリングを行う(ポリゴンの裏面を描画しないようにする)
	SetUseBackCulling(true);

	//変数の初期化
	m_deltaTime = 0.0f;
	m_timeScale = kTimeScale;

    return true;
}

void Application::Run()
{
	// 初期化
	long long previousTime = GetNowHiPerformanceCount();
	int targetFPS = kTargetFPS;
#if _DEBUG
	float totalTime = 0.0f;
#endif
	//ゲームループ
	while (ProcessMessage() != -1) // Windowsが行う処理を待つ
	{
		// 今フレームの開始時刻
		long long currentTime = GetNowHiPerformanceCount();
		m_deltaTime = static_cast<float>(currentTime - previousTime) / 1000000.0f;
		previousTime = currentTime;

		//画面全体をクリア
		ClearDrawScreen();
		//Widowモードが切り替わったかをチェック
		bool isWindow = m_isWindow;

		//ここにゲームの処理を書く
		
		
		//切り替わったなら
		if (m_isWindow != isWindow)
		{
			//切り替わり処理
			ChangeScreenMode();
		}
#if _DEBUG
		// 時間計測
		totalTime += m_deltaTime;
		DebugDrawFPS(totalTime, targetFPS);
#endif


		//画面の切り替わりを待つ必要がある
		ScreenFlip();//1/60秒経過するまで待つ

		// FPSに合わせて待機
		int frameMS = static_cast<int>(kMillisecond / targetFPS);
		long long frameEnd = GetNowHiPerformanceCount();
		float elapsedMS = static_cast<float>(frameEnd - previousTime) / kMillisecond;
		int waitMS = frameMS - static_cast<int>(elapsedMS);
		if (waitMS > 0) {
			WaitTimer(waitMS);
		}

		//ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) || m_isFinishApplication)
		{
			
			break;
		}
	}
}

void Application::Terminate()
{
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

void Application::SetWindowMode(bool isWindow)
{
	m_isWindow = isWindow;
}

void Application::ChangeScreenMode()
{
	//画面モード変更時(とウインドウモード変更時 )にグラフィックスシステムの設定やグラフィックハンドルをリセットするかどうかを設定する
	//Flag TRUE:リセットする(デフォルト)FALSE:リセットしない
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	//解像度
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	//切り替え
	ChangeWindowMode(m_isWindow);
	// 実際のウィンドウサイズも設定（フレーム込み）
	SetWindowSize(Game::kScreenWidth, Game::kScreenHeight);
	//て拡大率を1倍に戻す
	SetWindowSizeExtendRate(1.0);
	//画面全体をクリア
	ClearDrawScreen();
}

void Application::DebugDrawFPS(float totalTime, int targetFPS) const
{
	// デバッグ表示
	DrawFormatString(10, 10, 0xffffff, L"fps=%.2f", GetFPS());
	DrawFormatString(10, 30, 0xffffff, L"経過時間=%.2f", totalTime);
	DrawFormatString(10, 50, 0xffffff, L"targetFPS=%d", targetFPS);
}