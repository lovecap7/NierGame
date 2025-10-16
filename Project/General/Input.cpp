#include <Dxlib.h>
#include "Input.h"

namespace
{
	//トリガーの押し込み具合
	constexpr float kTriggerPower = 128;
	//スティックの入力成立の大きさ
	constexpr int kLeftStickPowerX = 20;
	constexpr int kLeftStickPowerY = 20;
	//少しだけ倒していると判定する範囲
	constexpr float kLowPowerStickMin = 50.0f;
	constexpr float kLowPowerStickMax = 400.0f;
	//そこそこ倒していると判定する範囲
	constexpr float kMediumPowerStickMin = kLowPowerStickMax;
	constexpr float kMediumPowerStickMax = 700.0f;
	//最大まで倒していると判定する範囲
	constexpr float kHighPowerStickMin = kMediumPowerStickMax;


	//先行入力
	//有効フレーム数
	constexpr int kBufferFrame = 10;
}

void Input::Init()
{
	m_inputActionMap["Ok"]		= { {InputType::kKeyboard,KEY_INPUT_RETURN}	, {InputType::kPad,PAD_INPUT_A} };
	m_inputActionMap["Pause"]	= { {InputType::kKeyboard,KEY_INPUT_P}		, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Start"]	= { {InputType::kKeyboard,KEY_INPUT_P}		, {InputType::kPad,PAD_INPUT_8} };
	m_inputActionMap["Select"]	= { {InputType::kKeyboard,KEY_INPUT_TAB}	, {InputType::kPad,PAD_INPUT_7} };
	m_inputActionMap["LS"]		= { {InputType::kKeyboard,KEY_INPUT_LSHIFT}	, {InputType::kPad,PAD_INPUT_START} };
	m_inputActionMap["RS"]		= { {InputType::kKeyboard,KEY_INPUT_RSHIFT}	, {InputType::kPad,PAD_INPUT_9} };
	m_inputActionMap["Cancel"]	= { {InputType::kKeyboard,KEY_INPUT_ESCAPE}	, {InputType::kPad,PAD_INPUT_B} };
	m_inputActionMap["Up"]		= { {InputType::kKeyboard,KEY_INPUT_W}		, {InputType::kPad,PAD_INPUT_UP} };
	m_inputActionMap["Down"]	= { {InputType::kKeyboard,KEY_INPUT_S}		, {InputType::kPad,PAD_INPUT_DOWN} };
	m_inputActionMap["Left"]	= { {InputType::kKeyboard,KEY_INPUT_A}		, {InputType::kPad,PAD_INPUT_LEFT} };
	m_inputActionMap["Right"]	= { {InputType::kKeyboard,KEY_INPUT_D}		, {InputType::kPad,PAD_INPUT_RIGHT} };
	m_inputActionMap["LB"]		= { {InputType::kKeyboard,KEY_INPUT_J}		, {InputType::kPad,PAD_INPUT_5} };
	m_inputActionMap["RB"]		= { {InputType::kKeyboard,KEY_INPUT_L}		, {InputType::kPad,PAD_INPUT_6} };
	m_inputActionMap["A"]		= { {InputType::kKeyboard,KEY_INPUT_U}		, {InputType::kPad,PAD_INPUT_1} };
	m_inputActionMap["B"]		= { {InputType::kKeyboard,KEY_INPUT_H}		, {InputType::kPad,PAD_INPUT_2} };
	m_inputActionMap["X"]		=	{ {InputType::kKeyboard,KEY_INPUT_G}	, {InputType::kPad,PAD_INPUT_3} };
	m_inputActionMap["Y"]		= { {InputType::kKeyboard,KEY_INPUT_Y}		, {InputType::kPad,PAD_INPUT_4} };
#if _DEBUG
	m_inputActionMap["DebugScene"] = { {InputType::kKeyboard,KEY_INPUT_D}};
	m_inputActionMap["AddFPS"] = { {InputType::kKeyboard,KEY_INPUT_6}};
	m_inputActionMap["SubFPS"] = { {InputType::kKeyboard,KEY_INPUT_5}};
	m_inputActionMap["AddTimeScale"] = { {InputType::kKeyboard,KEY_INPUT_8}};
	m_inputActionMap["SubTimeScale"] = { {InputType::kKeyboard,KEY_INPUT_7}};
	m_inputActionMap["StopUpdate"] = { {InputType::kKeyboard,KEY_INPUT_1}};
	m_inputActionMap["OneFrame"] = { {InputType::kKeyboard,KEY_INPUT_2}};
	m_inputActionMap["FullRecovery"] = { {InputType::kKeyboard,KEY_INPUT_3}};
	m_inputActionMap["Glitch"] = { {InputType::kKeyboard,KEY_INPUT_G}};
	m_inputActionMap["GlitchSub"] = { {InputType::kKeyboard,KEY_INPUT_H}};
	m_inputActionMap["Gray"] = { {InputType::kKeyboard,KEY_INPUT_J}};
	m_inputActionMap["GraySub"] = { {InputType::kKeyboard,KEY_INPUT_K}};
#endif

	//更新をする
	m_isUpdate = true;
}

void Input::Update()
{
	//前のフレームの入力情報を保存する
	m_lastInput = m_currentInput;

	if (!m_isUpdate)return;

	//すべての入力を取得する
	char keyState[256] = {};
	int padState = {};
	int mouseState = {};
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	mouseState = GetMouseInput();

	//アクション名に割り当てられているすべてのキーの入力をチェックする
	for (const auto& keyInfo : m_inputActionMap)
	{
		bool isPress = false;
		for (const auto& inputInfo : keyInfo.second)
		{
			//キーボードのチェック
			if (inputInfo.type == InputType::kKeyboard && keyState[inputInfo.buttonID])
			{
				isPress = true;
			}
			//パッドのチェック
			if (inputInfo.type == InputType::kPad && padState & inputInfo.buttonID)
			{
				isPress = true;
			}
			//マウスのチェック
			if (inputInfo.type == InputType::kMouse && mouseState & inputInfo.buttonID)
			{
				isPress = true;
			}

			if (isPress)
			{
				//入力があるならカウント
				m_repeateCountFrame[keyInfo.first]++;
				break;
			}
		}
		//現在のフレームで押されていたかどうかを返す
		m_currentInput[keyInfo.first] = isPress;
		if (!isPress)
		{
			//押されてないならリセット
			m_repeateCountFrame[keyInfo.first] = 0;
		}
	}

	//スティックの入力を初期化する
	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	//スティックの入力を取得する
	DxLib::GetJoypadAnalogInput(&m_stickInfo.leftStickX, &m_stickInfo.leftStickY, DX_INPUT_PAD1);
	DxLib::GetJoypadAnalogInputRight(&m_stickInfo.rightStickX, &m_stickInfo.rightStickY, DX_INPUT_PAD1);
	XINPUT_STATE* xInputState = new XINPUT_STATE;
	DxLib::GetJoypadXInputState(DX_INPUT_PAD1, xInputState);
	m_triggerInfo.left = xInputState->LeftTrigger;
	m_triggerInfo.right = xInputState->RightTrigger;
	delete xInputState;


	//先行入力を更新
	UpdateBuffer();

	//新しく押された入力をバッファに登録
	for (const auto& keyInfo : m_currentInput)
	{
		if (IsTrigger(keyInfo.first))
		{
			m_inputBufferFrame[keyInfo.first] = kBufferFrame;
		}
	}
}

void Input::StopUpdate()
{
	for (auto& item : m_currentInput)
	{
		item.second = false;
	}
	for (auto& item : m_lastInput)
	{
		item.second = false;
	}

	m_stickInfo.leftStickX = 0;
	m_stickInfo.leftStickY = 0;
	m_stickInfo.rightStickX = 0;
	m_stickInfo.rightStickY = 0;

	m_triggerInfo.left = 0;
	m_triggerInfo.right = 0;

	//更新をとめる
	m_isUpdate = false;
}

bool Input::IsPress(const std::string& action)const
{
	auto keyInfo = m_currentInput.find(action);

	//ボタン名が定義されていなかったらfalseを返す
	if (keyInfo == m_currentInput.end())
	{
		return false;
	}
	else
	{
		//見つかったらboolの値を返す
		return keyInfo->second;
	}
}

bool Input::IsTrigger(const std::string& action)const
{
	if (IsPress(action))
	{
		//前のフレームを参照
		auto last = m_lastInput.find(action);
		//未定義のボタン名が来たらfalseを返す
		if (last == m_lastInput.end())
		{
			return false;
		}
		else
		{
			//前のフレームでも押されていたら
			return !last->second;
		}
	}
	else
	{
		return false;
	}
}

bool Input::IsRelease(const std::string& action)const
{
	//まず押されているかを判定
	if (IsPress(action))
	{
		//押されていたらfalseを返す
		return false;
	}
	else
	{
		//前のフレームを参照
		auto last = m_lastInput.find(action);
		//押されていたらfalseを返す
		if (last == m_lastInput.end())
		{
			return false;
		}
		//押されていなければ
		else
		{
			return last->second;
		}
	}

	return false;
}

bool Input::IsLowPowerLeftStick()const
{
	//少しだけ倒しているならtrue
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kLowPowerStickMin && length < kLowPowerStickMax)
	{
		return true;
	}
	return false;
}

bool Input::IsMediumPowerLeftStick()const
{
	//そこそこ倒しているならtrue
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kMediumPowerStickMin && length < kMediumPowerStickMax)
	{
		return true;
	}
	return false;
}

bool Input::IsHighPowerLeftStick() const
{
	//倒し切ってるならtrue
	Vector2 stickVec;
	stickVec.x = m_stickInfo.leftStickX;
	stickVec.y = m_stickInfo.leftStickY;
	float length = stickVec.Magnitude();
	if (length >= kHighPowerStickMin)
	{
		return true;
	}
	return false;
}


bool Input::IsPushTrigger(bool right, int power)
{
	//トリガーの状態取得
	auto trigger = GetTriggerInfo();

	//右側を判定する場合
	if (right)
	{
		//引数よりも強く押し込まれたら
		if (trigger.right >= power)
		{
			return true;
		}
	}
	//左側を判定する場合
	else
	{
		//引数よりも強く押し込まれたら
		if (trigger.left >= power)
		{
			return true;
		}
	}
	return false;
}

bool Input::IsPushTrigger(bool right)
{
	return IsPushTrigger(right, kTriggerPower);
}

bool Input::IsPressAny()const
{
	return	IsPress("Ok")		||
			IsPress("Pause")	||
			IsPress("Start")	||
			IsPress("Select")	||
			IsPress("LS")		||
			IsPress("RS")		||
			IsPress("Cancel")	||
			IsPress("LB")		||
			IsPress("RB")		||
			IsPress("A")		||
			IsPress("B")		||
			IsPress("X")		||
			IsPress("Y");
}

bool Input::IsTriggerAny()const
{
	return	IsTrigger("Ok")		||
			IsTrigger("Pause")	||
			IsTrigger("Start")	||
			IsTrigger("Select") ||
			IsTrigger("LS")		||
			IsTrigger("RS")		||
			IsTrigger("Cancel") ||
			IsTrigger("LB")		||
			IsTrigger("RB")		||
			IsTrigger("A")		||
			IsTrigger("B")		||
			IsTrigger("X")		||
			IsTrigger("Y");
}

bool Input::IsRepeate(const std::string& action, int deltaTime)const
{
	//入力が入ったら
	if (IsPress(action))
	{
		//一定間隔で返す
		if (m_repeateCountFrame.at(action) % deltaTime == 0 || IsTrigger(action))
		{
			return true;
		}
	}
	return false;
}

bool Input::IsBuffered(const std::string& action) const
{
	auto it = m_inputBufferFrame.find(action);
	if (it == m_inputBufferFrame.end()) return false;

	//まだ有効時間内ならtrue
	return it->second > 0;
}

void Input::UpdateBuffer()
{
	for (auto& [key, frame] : m_inputBufferFrame)
	{
		//フレームを減らしていく
		if (frame > 0)
		{
			--frame;
		}
	}
}
