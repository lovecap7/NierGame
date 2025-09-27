#include<DxLib.h>
#include<cassert>
#include<array>
#include<memory>
#include"General/Model.h"
#include"General/Math/MyMath.h"
#include"General/ShaderDrawUtil.h"
#include "General/ShaderPostProcess.h"
#include "General/Game.h"
namespace
{
	//ターゲットFPS
	const int kTargetFPS = 120;
	//ミリ秒
	float kMillisecond = 1000.0f;
	//マイクロ秒
	float kMicrosecond = 1000000.0f;
	//タイムスケール
	float kTimeScale = 1.0f;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, 32);
	if (DxLib_Init() != 0) {
		return -1;
	}

	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetDrawMode(DX_DRAWMODE_NEAREST);

	//3D描画のための準備
	//Zバッファを使用する
	SetUseZBuffer3D(true);
	//Zバッファに書き込みを行う
	SetWriteZBuffer3D(true);
	//バックカリングを行う(ポリゴンの裏面を描画しないようにする)
	SetUseBackCulling(true);

	int bgH = LoadGraph(L"img/bg.png");
	assert(bgH >= 0);
	//ハンドル
	int handle = LoadGraph(L"img/ジャスト回避.png");
	assert(handle >= 0);
	//ポストエフェクト
	int psH = LoadPixelShader(L"Shader/2D/PostEffects.pso");
	assert(psH >= 0);

	SetDrawScreen(DX_SCREEN_BACK);
	
	//ポストエフェクトの準備
	auto m_postProcess = std::make_unique<ShaderPostProcess>();
	m_postProcess->Init();
	//レンダーターゲット(Drawの書き込み先)
	auto RT = MakeScreen(Game::kScreenWidth, Game::kScreenHeight);
	// 初期化
	long long previousTime = GetNowHiPerformanceCount();
	int targetFPS = 120;
#if _DEBUG
	float totalTime = 0.0f;
#endif

	float deltaTime = 0.0f;
	//ゲームループ
	while (ProcessMessage() != -1) // Windowsが行う処理を待つ
	{
		// 今フレームの開始時刻
		long long currentTime = GetNowHiPerformanceCount();
		deltaTime = static_cast<float>(currentTime - previousTime) / kMicrosecond;//秒に変換
		previousTime = currentTime;

		//ターゲット
		SetDrawScreen(RT);
		//画面全体をクリア
		ClearDrawScreen();
		
		//ここにゲームの処理を書く
		m_postProcess->Update();

		DrawGraph(0, 0, handle, true);
		DrawBox(200, 400, 300, 500, GetColor(0, 0, 0), true);
		
		//裏描画
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		//裏画面にレンダーターゲットを描画
		m_postProcess->Draw(RT);
#if _DEBUG
		// 時間計測
		totalTime += deltaTime;
		// デバッグ表示
		DrawFormatString(10, 10, 0xffffff, L"fps=%.2f", GetFPS());
		DrawFormatString(10, 30, 0xffffff, L"経過時間=%.2f", totalTime);
		DrawFormatString(10, 50, 0xffffff, L"targetFPS=%d", targetFPS);
#endif

		//画面の切り替わりを待つ必要がある
		ScreenFlip();

		// FPSに合わせて待機
		int frameMS = static_cast<int>(kMillisecond / targetFPS);
		long long frameEnd = GetNowHiPerformanceCount();
		float elapsedMS = static_cast<float>(frameEnd - previousTime) / kMillisecond;
		int waitMS = frameMS - static_cast<int>(elapsedMS);
		if (waitMS > 0) {
			WaitTimer(waitMS);
		}

		//ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{

			break;
		}
	}
	DxLib_End();
	return 0;
}