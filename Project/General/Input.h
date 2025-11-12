#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>
#include "Math/MyMath.h"

class Input final
{
private:
	//シングルトンの準備
	Input() = default;
	~Input() = default;
	//コピー禁止
	Input(const Input&) = delete;
	Input& operator = (const Input&) = delete;
	//ムーブ禁止
	Input(Input&&) = delete;
	Input& operator = (Input&&) = delete;
public:
	/// <summary>
	/// シングルトンインスタンスを取得する
	/// </summary>
	/// <returns></returns>
	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

	/// <summary>
	/// スティックの傾きを保持する構造体
	/// </summary>
	struct StickInfo
	{
		/// <summary>
		/// 左(Max -1000)
		/// 右(Max  1000)
		/// </summary>
		int leftStickX;
		/// <summary>
		/// 上(Max -1000)
		/// 下(Max  1000)
		/// </summary>
		int leftStickY;
		/// <summary>
		/// 左(Max -1000)
		/// 右(Max  1000)
		/// </summary>
		int rightStickX;
		/// <summary>
		/// 上(Max -1000)
		/// 下(Max  1000)
		/// </summary>
		int rightStickY;

		// 前フレームの値を保存
		int prevRightStickX;
		int prevRightStickY;

		StickInfo()
		{
			leftStickX = 0;
			leftStickY = 0;
			rightStickX = 0;
			rightStickY = 0;
			prevRightStickX;
			prevRightStickY;
		}
		//入力が入ったかどうかを取得(移動状態に切り替えるときに使う)
		bool IsLeftStickInput() { return leftStickX != 0 || leftStickY != 0; };
		bool IsRightStickInput() { return rightStickX != 0 || rightStickY != 0; };

		//入力の大きさ
		float LeftStickMagnitude() const
		{
			return Vector2(static_cast<float>(leftStickX), static_cast<float>(leftStickY)).Magnitude();
		}
		float RightStickMagnitude() const
		{
			return Vector2(static_cast<float>(rightStickX), static_cast<float>(rightStickY)).Magnitude();
		}

		//割合
		float LeftStickRate() const
		{
			return LeftStickMagnitude() * 0.001f;
		}
		float RightStickRate() const
		{
			return RightStickMagnitude() * 0.001f;
		}

		//最後の入力を保持
		void UpdatePrev()
		{
			prevRightStickX = rightStickX;
			prevRightStickY = rightStickY;
		}

		//右スティックが倒された瞬間
		bool IsRightStickTrigger() const
		{
			float prevMag = Vector2((float)prevRightStickX, (float)prevRightStickY).Magnitude();
			float currMag = RightStickMagnitude();

			//前のフレームで入力がなく今あるなら
			return (prevMag <= 0.0f) && (currMag > 0.0f);
		}
	};

	struct TriggerInfo
	{
		TriggerInfo()
		{
			left = 0;
			right = 0;
		}
		int left;
		int right;
	};

	/// <summary>
	/// キーの情報を更新する
	/// </summary>
	void Init();

	/// <summary>
	/// 入力情報を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// 入力情報を受け付けないようにする
	/// </summary>
	void StopUpdate();

	/// <summary>
	/// 入力情報を受け付けないようにする
	/// </summary>
	void StartUpdate() { m_isUpdate = true; };

	/// <summary>
	/// キー押したかどうか
	/// </summary>
	/// <param name="keyName">判定したいキー</param>
	// <returns>押したかどうか</returns>
	bool IsPress(const std::string& action)const;

	/// <summary>
	/// キーを押された瞬間を取得する
	/// </summary>
	/// <param name="keyName">判定したいキー</param>
	/// <returns>押された瞬間のみtrue</returns>
	bool IsTrigger(const std::string& action)const;

	/// <summary>
	/// キーを離した瞬間を取得する
	/// </summary>
	/// <param name="keyName">判定したいキー</param>
	/// <returns>離された瞬間のみtrue</returns>
	bool IsRelease(const std::string& action)const;

	/// <summary>
	/// スティックの傾きを取得する
	/// </summary>
	/// <returns>スティックがどれだけ倒れているか</returns>
	StickInfo GetStickInfo()const { return m_stickInfo; }

	/// <summary>
	/// 少しだけ倒しているならtrue
	/// </summary>
	/// <returns></returns>
	bool IsLowPowerLeftStick()const;
	/// <summary>
	/// 真ん中ぐらいで倒したらtrue
	/// </summary>
	/// <returns></returns>
	bool IsMediumPowerLeftStick()const;
	/// <summary>
	/// 最大まで倒したら
	/// </summary>
	/// <returns></returns>
	bool IsHighPowerLeftStick()const;

	/// <summary>
	/// トリガーの押し込み具合を取得する
	/// </summary>
	/// <returns>トリガーがどのくらい押し込まれているか</returns>
	TriggerInfo GetTriggerInfo() { return m_triggerInfo; }

	/// <summary>
	/// トリガーボタンを押しているかどうかを返す(押し込みの強さ可変)
	/// </summary>
	/// <param name="right">右側のボタンを取得したいならtrue</param>
	/// <param name="power">どのくらい押し込まれていたらtrueとするか(最大255,最小0)</param>
	/// <returns></returns>
	bool IsPushTrigger(bool right, int power);

	/// <summary>
	/// トリガーボタンを押しているかどうかを返す(押し込みの強さ固定)
	/// </summary>
	/// <param name="right"></param>
	/// <returns></returns>
	bool IsPushTrigger(bool right);

	/// <summary>
	/// 何かキーやボタンを押したときにtrue
	/// </summary>
	/// <returns></returns>
	bool IsPressAny()const;

	/// <summary>
	/// 何かキーかボタンを押した瞬間true
	/// </summary>
	/// <returns></returns>
	bool IsTriggerAny()const;

	/// <summary>
	/// 入力があった時一定間隔でtrueを返す
	/// </summary>
	/// <returns></returns>
	bool IsRepeate(const std::string& action,int deltaTime = 30)const;

	/// <summary>
   /// 先行入力をチェックする
   /// </summary>
   /// <param name="action">アクション名</param>
   /// <returns>先行入力が有効ならtrue</returns>
	bool IsBuffered(const std::string& action);

private:
	/// <summary>
	/// 入力の種類
	/// </summary>
	enum class InputType
	{
		kKeyboard,
		kPad,
		kMouse
	};
	struct InputMapInfo
	{
		InputType type;
		int buttonID;
	};

	std::map<std::string, std::vector<InputMapInfo>> m_inputActionMap;
	//現在のフレームの入力
	std::map<std::string, bool>m_currentInput;
	//1フレーム前の入力
	std::map<std::string, bool>m_lastInput;
	//入力し続けてた時のフレームカウント
	std::map<std::string, int>m_repeateCountFrame;
	//スティックに関する入力情報
	StickInfo m_stickInfo = StickInfo();
	//トリガーの状態
	TriggerInfo m_triggerInfo = TriggerInfo();
	//更新するか
	bool m_isUpdate = true;

	//先行入力
	std::map<std::string, int> m_inputBufferFrame;

private:
	/// <summary>
	/// すべての先行入力を更新する（フレームごとに呼ばれる）
	/// </summary>
	void UpdateBuffer();
};


