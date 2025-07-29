//---------------------------------------
//
//入力処理[input.cpp]
//Author fuma sato
//
//---------------------------------------
#include"input.h"

#include <cstddef> // offsetof を使うために必要

using namespace input; // インプット空間の使用

//----------------------------------
//
// インプットクラス (呼び出し用窓口)
//
//----------------------------------

//-----------------
// 初期化
//-----------------
HRESULT CInput::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// キーボード
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboardManager;

		if (m_pKeyboard != nullptr)
		{
			if (FAILED(m_pKeyboard->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// マウス
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouseManager;

		if (m_pMouse != nullptr)
		{
			if (FAILED(m_pMouse->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// コントローラー
	if (m_pController == nullptr)
	{
		m_pController = new CInputControllerManager;

		if (m_pController != nullptr)
		{
			if (FAILED(m_pController->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	return S_OK;
}

//-----------------
// 終了
//-----------------
void CInput::Uninit(void)
{
	// キーボード
	SAFE_UNINIT(m_pKeyboard);

	// マウス
	SAFE_UNINIT(m_pMouse);

	// コントローラー
	SAFE_UNINIT(m_pController);
}

//-----------------
// 更新
//-----------------
void CInput::Update(void)
{
	// キーボード
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// マウス
	if (m_pMouse != nullptr)
	{
		m_pMouse->Update();
	}

	// コントローラー
	if (m_pController != nullptr)
	{
		m_pController->Update();
	}
}

//-----------------
// 押している
//-----------------
bool CInput::IsPress(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsPress(idx, button) || m_pMouse->IsPress(idx, button) || m_pController->IsPress(idx, button);
}

//-----------------
// 押した瞬間
//-----------------
bool CInput::IsTrigger(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsTrigger(idx, button) || m_pMouse->IsTrigger(idx, button) || m_pController->IsTrigger(idx, button);
}

//-----------------
// 離した瞬間
//-----------------
bool CInput::IsRelease(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsRelease(idx, button) || m_pMouse->IsRelease(idx, button) || m_pController->IsRelease(idx, button);
}

//-----------------
// 長押し
//-----------------
bool CInput::IsRepeat(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsRepeat(idx, button) || m_pMouse->IsRepeat(idx, button) || m_pController->IsRepeat(idx, button);
}

//-----------------
// 長押し
//-----------------
size_t CInput::Count(void)
{
	return m_pKeyboard->Count() + m_pMouse->Count() + m_pController->Count();
}

//--------------------------
//
// キーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputKeyboardManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputKeyboard* pKeyboard{}; // キーボードポインタ

	// raw
	do
	{// ループ
		if (pKeyboard != nullptr) return E_FAIL;  // null
		pKeyboard = new CInputRawInputKeyboard;   // rawInput生成
		if (pKeyboard == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd)))
		{// 成功
			m_apKeyboard.push_back(pKeyboard); // キーボード追加
			pKeyboard = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pKeyboard); // 捨てる
			break;
		}
	} while (true);

	if (m_apKeyboard.size() > 0) { return S_OK; } // キーボード生成済み

	// Direct
	if (pKeyboard != nullptr) return E_FAIL;   // null
	pKeyboard = new CInputDirectInputKeyboard; // DirectInput生成
	if (pKeyboard == nullptr)  return E_FAIL;  // 生成失敗

	// 初期化
	if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd)))
	{// 成功
		m_apKeyboard.push_back(pKeyboard); // キーボード追加
		return S_OK;
	}
	SAFE_UNINIT(pKeyboard); // 捨てる
	return E_FAIL;
}

//----------------------
//終了処理
//----------------------
void CInputKeyboardManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apKeyboard); // キーボードをすべて破棄
}

//-------------------------
//更新処理
//-------------------------
void CInputKeyboardManager::Update(void)
{
	for (auto& pKeyboard : m_apKeyboard)
	{// 全てのキーボードの更新
		pKeyboard->Update();
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputKeyboardManager::IsPress(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputKeyboardManager::IsTrigger(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputKeyboardManager::IsRelease(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputKeyboardManager::IsRepeat(Index8 idx, BUTTON button)
{
	return true;
}

//--------------------------
//
// マウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputMouseManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputMouse* pMouse{}; // マウスポインタ

	// raw
	do
	{// ループ
		if (pMouse != nullptr) return E_FAIL;  // null
		pMouse = new CInputRawInputMouse;      // rawInput生成
		if (pMouse == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pMouse->Init(hInstanse, hWnd)))
		{// 成功
			m_apMouse.push_back(pMouse); // マウス追加
			pMouse = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pMouse); // 捨てる
			break;
		}
	} while (true);

	if (m_apMouse.size() > 0) { return S_OK; } // マウス生成済み

	// Direct
	if (pMouse != nullptr) return E_FAIL;  // null
	pMouse = new CInputDirectInputMouse;   // DirectInput生成
	if (pMouse == nullptr)  return E_FAIL; // 生成失敗

	// 初期化
	if (SUCCEEDED(pMouse->Init(hInstanse, hWnd)))
	{// 成功
		m_apMouse.push_back(pMouse); // マウス追加
		return S_OK;
	}
	SAFE_UNINIT(pMouse); // 捨てる
	return E_FAIL;
}

//----------------------
//終了処理
//----------------------
void CInputMouseManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apMouse); // マウスをすべて破棄
}

//-------------------------
//更新処理
//-------------------------
void CInputMouseManager::Update(void)
{
	for (auto& pMouse : m_apMouse)
	{// 全てのマウスの更新
		pMouse->Update();
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputMouseManager::IsPress(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputMouseManager::IsTrigger(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputMouseManager::IsRelease(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputMouseManager::IsRepeat(Index8 idx, BUTTON button)
{
	return true;
}

//--------------
//マウス移動
//--------------
Axis CInputMouseManager::GetAxis(Index8 idx)
{
	return Axis();
}

//----------------
//キー押下処理
//----------------
float CInputMouseManager::GetWheel(Index8 idx)
{
	return float{};
}

//------------------------------------
//
// コントローラークラス
//
//------------------------------------

//-------------------
// 初期化
//-------------------
HRESULT CInputControllerManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputController* pController{}; // コントローラーポインタ

	// X
	XInputEnable(true); // XInput有効化
	for (Index8 cnt = 0; cnt < 4u; cnt++)
	{// XInput確認
		if (pController != nullptr) return E_FAIL;  // null
		pController = new CInputXInputController;   // XInput生成
		if (pController == nullptr)  return E_FAIL; // 生成失敗

		// 初期化
		if (SUCCEEDED(pController->Init(hInstanse, hWnd, cnt)))
		{// 成功
			m_apController.push_back(pController); // コントローラー追加
			pController = nullptr;                 // null
			continue;                              // 続ける
		}
		SAFE_UNINIT(pController); // 捨てる
	}

	size_t XInputNum = m_apController.size(); // Xinputの数
	if (XInputNum < 1)
	{// Xinputコントローラーがなかった
		XInputEnable(false); // XInput無効化
	}

	// Direct

	// 仮登録
	if (FAILED(CInputDirectInput::Register(hInstanse)))return E_FAIL;
	// デバイス列挙
	if (FAILED(CInputDirectInput::SetEnum(this)))return E_FAIL;
	// 仮登録解除
	CInputDirectInput::Unregister();

	for (Index cnt = XInputNum; cnt < m_apController.size(); cnt++)
	{// DirectInputを確認
		// 初期化
		if (FAILED(m_apController[cnt]->Init(hInstanse, hWnd)))
		{// 失敗
			SwapRemove(m_apController, cnt); // 削除
		}
	}

	// メモリを合わせる希望
	m_apController.shrink_to_fit();

	return S_OK;
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apController); // コントローラーをすべて破棄
	XInputEnable(false); // XInput無効化
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::Update(void)
{
	for (auto& pController : m_apController)
	{// 全てのコントローラーの更新
		pController->Update();
	}
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsPress(Index idx, BUTTON button)
{
	return true;
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsTrigger(Index idx, BUTTON button)
{
	return true;
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsRelease(Index idx, BUTTON button)
{
	return true;
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsRepeat(Index idx, BUTTON button)
{
	return true;
}

//-------------------
// 初期化
//-------------------
void CInputControllerManager::SetVibrate(Index idx, float fLeftPower, float fReghtPower)
{
	m_apController[idx]->Vibrate(fLeftPower, fReghtPower);
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsXInputControllerConnected(Index8 idx)
{
	XINPUT_STATE state{};
	if (idx == INVALID_ID)
	{
		for (Index8 cnt = 0; cnt < 3; ++cnt)
		{
			if (XInputGetState(cnt, &state) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(idx, &state) == ERROR_SUCCESS;
}

//-------------------
// 初期化
//-------------------
bool CInputControllerManager::IsDirectInputControllerConnected(Index8 idx)
{
	if (idx == INVALID_ID)
	{
		for (Index8 cnt = 0; cnt < m_apController.size(); ++cnt)
		{
			if (m_apController[cnt] != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	return m_apController[idx] != nullptr;
}

//--------------------------
//
// キーボード基底
//
//--------------------------

//------
// 更新
//------
void CInputKeyboard::Update(void)
{
	m_keyOld = m_key; // 前回のフレームを保存

	array <bool, input::MAX_KEY> key{};
	if (SUCCEEDED(GetKey(key)))
	{
		for (Index8 cntKey = 0; cntKey < key.size(); cntKey++)
		{
			m_key[cntKey] = key[cntKey]; //一時的な情報を正規情報に渡す

			bool now = m_key[cntKey];    // 今押しているかどうか
			bool old = m_keyOld[cntKey]; // 前回押していたかどうか

			m_bPress[cntKey] = now;           // 押
			m_bTrigger[cntKey] = now && !old; // 押下
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME); // リリース

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// キーの変化
				// タイムの初期化
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// 押している
				// タイムの増加
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// リピート中
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now;
					}
				}
			}
		}
	}
}

//--------------------------
//
// マウス基底
//
//--------------------------

//------
// 更新
//------
void CInputMouse::Update(void)
{
	m_buttonOld = m_button; // 前回のフレームを保存

	// ボタンの状態を解析
	array<bool, Index8(input::MOUSE_BUTTON::Max)> button{};
	if (SUCCEEDED(GetButton(button)))
	{
		for (Index8 cntKey = 0; cntKey < button.size(); cntKey++)
		{
			m_button[cntKey] = button[cntKey]; //一時的な情報を正規情報に渡す

			bool now = m_button[cntKey];    // 今押しているかどうか
			bool old = m_buttonOld[cntKey]; // 前回押していたかどうか

			m_bPress[cntKey] = now;           // 押
			m_bTrigger[cntKey] = now && !old; // 押下
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME); // リリース

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// キーの変化
				// タイムの初期化
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// 押している
				// タイムの増加
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// リピート中
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now;
					}
				}
			}
		}
	}

	GetMove(&m_Move); 	    // 動きの状態をもらう
	GetWheel(&m_WheelMove); // ホイールの状態をもらう
}

//--------------------------
//
// コントローラー基底
//
//--------------------------

//------
// 更新
//------
void CInputController::Update(void)
{
	m_buttonOld = m_button; // 前回のフレームを保存

	// ボタンの状態を解析
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> button{};
	if (SUCCEEDED(GetButton(button)))
	{
		for (Index8 cntKey = 0; cntKey < button.size(); cntKey++)
		{
			m_button[cntKey] = button[cntKey]; //一時的な情報を正規情報に渡す

			bool now = m_button[cntKey];    // 今押しているかどうか
			bool old = m_buttonOld[cntKey]; // 前回押していたかどうか

			m_bPress[cntKey] = now;           // 押
			m_bTrigger[cntKey] = now && !old; // 押下
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME); // リリース

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// キーの変化
				// タイムの初期化
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// 押している
				// タイムの増加
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// リピート中
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now;
					}
				}
			}
		}
	}

	GetStick(m_Stick); 	   // スティックの状態をもらう
	GetTrigger(m_Trigger); // トリガーの状態をもらう
}

//--------------------------
//
// rawInput母体
//
//--------------------------

//---------------------------------------------
// メッセージから貰ったデータをデバイスに送信
//---------------------------------------------
HRESULT CInputRawInput::AnalysisRawData(RAWINPUT* pRawData)
{
	return S_OK;
}

//--------------------------
//
// rawInputキーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputRawInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{


	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputRawInputKeyboard::Uninit(void)
{

}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputKeyboard::GetKey(span<bool> key) const
{
	return S_OK;
}

//--------------------------
//
// rawInputマウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputRawInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{
	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputRawInputMouse::Uninit(void)
{

}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetButton(span<bool> button) const
{
	return S_OK;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetMove(input::Axis* pMove) const
{
	return S_OK;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputRawInputMouse::GetWheel(float* pWheel) const
{
	return S_OK;
}

//--------------------------
//
//パッド
//
//--------------------------

//--------------------------------
//初期化処理
//--------------------------------
HRESULT CInputXInputController::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	m_idx = idx; // インデックスを保存

	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		return S_OK;
	}
	return E_FAIL;
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetButton(span<bool> button) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		SetTriggerButton(&state);
		for (Index8 cntButton = 0; cntButton < Index8(JOYKEY::Max); cntButton++)
		{
			// 変換表からXInput番号からコントローラー共通の番号を取得する
			Index8 controllerButton = Index8(ControllerMapList[Index8(CONTROLLER_TYPE::XInput)][cntButton]);

			// JOYKEYからXInputようのMaskに変換
			WORD mask = GetXInputMask(JOYKEY(cntButton));

			// ボタンを押しているかどうかを取得する
			button[controllerButton] = (state.Gamepad.wButtons & mask) != 0;
		}
		return S_OK;
	}
	return E_FAIL;
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetStick(span<input::Axis> axis) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		// 左スティック
		axis[Left].x = float(state.Gamepad.sThumbLX);
		axis[Left].y = float(state.Gamepad.sThumbLY);

		// 右スティック
		axis[Right].x = float(state.Gamepad.sThumbRX);
		axis[Right].y = float(state.Gamepad.sThumbRY);

		return S_OK;
	}

	return E_FAIL;
}

//------------------------
//更新処理
//------------------------
HRESULT CInputXInputController::GetTrigger(span<float> trigger) const
{
	constexpr float TRIGGER_MAX = float(numeric_limits<BYTE>::max());

	XINPUT_STATE XState;
	if (XInputGetState(m_idx, &XState) == ERROR_SUCCESS)
	{
		// 左スティック
		trigger[Left] = float(XState.Gamepad.bLeftTrigger) / TRIGGER_MAX;

		// 右スティック
		trigger[Right] = float(XState.Gamepad.bRightTrigger) / TRIGGER_MAX;

		return S_OK;
	}

	return E_FAIL;
}

//------------------------
// スライダー
//------------------------
HRESULT CInputXInputController::GetSlider(span<float> slider) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		// XInputのスライダーはスティックのY軸を入れておく
		slider[Left] = float(state.Gamepad.sThumbLY) / float(STICK_NUM);
		slider[Right] = float(state.Gamepad.sThumbRY) / float(STICK_NUM);

		return S_OK;
	}

	return E_FAIL;
}

//----------------
//振動
//----------------
void CInputXInputController::Vibrate(float leftMotorSpeed, float rightMotorSpeed)
{
	constexpr WORD VIBRATION_MAX = numeric_limits<WORD>::max();
	leftMotorSpeed = clamp(leftMotorSpeed, 0.0f, 1.0f);
	rightMotorSpeed = clamp(rightMotorSpeed, 0.0f, 1.0f);
	XINPUT_VIBRATION vibration{ WORD(float(VIBRATION_MAX) * leftMotorSpeed) ,WORD(float(VIBRATION_MAX) * rightMotorSpeed) };
	XInputSetState(0, &vibration);
}

//-----------------------------------------------
// トリガーの値が閾値を超えたらボタンに入力に送る
//-----------------------------------------------
void CInputXInputController::SetTriggerButton(PXINPUT_STATE pState) const
{
	// トリガーの閾値を確認
	if (pState->Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{// 超えていたら
		// wButtonsのビットに格納
		pState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_TRIGGER;
	}
	if (pState->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{// 超えていたら
		// wButtonsのビットに格納
		pState->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_TRIGGER;
	}
}

//-----------------------------------------------
// 列挙型をXInput本来のマクロに置き換える
//-----------------------------------------------
WORD CInputXInputController::GetXInputMask(input::JOYKEY key) const
{
	switch (key)
	{
	case JOYKEY::Up:    return XINPUT_GAMEPAD_DPAD_UP;
	case JOYKEY::Down:  return XINPUT_GAMEPAD_DPAD_DOWN;
	case JOYKEY::Left:  return XINPUT_GAMEPAD_DPAD_LEFT;
	case JOYKEY::Right: return XINPUT_GAMEPAD_DPAD_RIGHT;
	case JOYKEY::Start: return XINPUT_GAMEPAD_START;
	case JOYKEY::Back:  return XINPUT_GAMEPAD_BACK;
	case JOYKEY::L3:    return XINPUT_GAMEPAD_LEFT_THUMB;
	case JOYKEY::R3:    return XINPUT_GAMEPAD_RIGHT_THUMB;
	case JOYKEY::LB:    return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case JOYKEY::RB:    return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case JOYKEY::LT:    return XINPUT_GAMEPAD_LEFT_TRIGGER;
	case JOYKEY::RT:    return XINPUT_GAMEPAD_RIGHT_TRIGGER;
	case JOYKEY::A:     return XINPUT_GAMEPAD_A;
	case JOYKEY::B:     return XINPUT_GAMEPAD_B;
	case JOYKEY::X:     return XINPUT_GAMEPAD_X;
	case JOYKEY::Y:     return XINPUT_GAMEPAD_Y;
	default:            return 0;
	}
}

//----------------------------------
//
// DirectInput母体
//
//----------------------------------

LPDIRECTINPUT8 CInputDirectInput::m_pInput{};
size_t CInputDirectInput::m_useCount{};

//-----------------
// 初期化
//-----------------
HRESULT CInputDirectInput::Register(HINSTANCE hInstanse)
{
	//インプットオブジェクトの作成
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create
		(
			hInstanse,
			DIRECTINPUT_VERSION, IID_IDirectInput8,
			(void**)&m_pInput,
			nullptr
		)))
		{
			return E_FAIL;
		}
	}

	// 参照追加
	m_useCount++;

	return S_OK;
}

//-----------------
// 破棄
//-----------------
void CInputDirectInput::Unregister(void)
{
	m_useCount--; // DirectInput参照数

	if (m_useCount <= 0)
	{// 参照数が0以下
		//インプットオブジェクトの破棄
		SAFE_RELEASE(m_pInput);
	}
}

//-----------------
// 列挙
//-----------------
HRESULT CInputDirectInput::SetEnum(CInputControllerManager* pControllerManager)
{
	//インプットオブジェクトの作成
	if (m_pInput != nullptr)
	{
		// DirectInputデバイスを列挙
		return m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, pControllerManager, DIEDFL_ATTACHEDONLY);
	}
	return E_FAIL;
}

//---------------------------
//デバイス情報取得
//---------------------------
BOOL CALLBACK CInputDirectInput::EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// CInputControllerManagerを抽出する
	CInputControllerManager* pControllerManager = static_cast<CInputControllerManager*>(pvRef);

	// DirectInputコントローラーの生成
	CInputDirectInputController* pdController = new CInputDirectInputController;

	// デバイス情報を渡す
	pdController->SetInstance(lpddi);

	// 基底クラスポインタにアップ (安全)
	CInputController* pController = pdController;

	// Managerに渡す
	pControllerManager->Register(pController);

	// 列挙を続行
	return DIENUM_CONTINUE;
}

//--------------------------
//
// DirectInputキーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{
	//インプットデバイスの作成 
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr))) return E_FAIL;

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// デバイス設定
	if (FAILED(SetProperty()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse);

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputKeyboard::Uninit(void)
{
	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// DirectInput母体への登録解除
	CInputDirectInput::Unregister();
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputKeyboard::GetKey(span<bool> key) const
{
	if (m_pDevice != nullptr)
	{
		BYTE state[MAX_KEY];//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(state), state)))
		{
			for (Index8 cntKey = 0; cntKey < key.size(); cntKey++)
			{
				key[cntKey] = (state[cntKey] & 0x80) != 0;
			}
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//------------------------
// デバイス設定
//------------------------
HRESULT CInputDirectInputKeyboard::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// デバイス全体に設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16個のイベントを格納できるバッファを作成
	dipdw.dwData = 16;
	return m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
}

//--------------------------
//
// DirectInputマウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{
	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))return E_FAIL;

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// デバイス設定
	if (FAILED(SetProperty()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse);

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputMouse::Uninit(void)
{
	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// インプットの破棄
	CInputDirectInput::Unregister();
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetButton(span<bool> button) const
{
	if (m_pDevice != nullptr)
	{
		DIMOUSESTATE2 state{};//マウス情報//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			for (Index8 cntButton = 0; cntButton < button.size(); cntButton++)
			{
				button[cntButton] = (state.rgbButtons[cntButton] & 0x80) != 0;
			}
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetMove(input::Axis* pMove) const
{
	if (m_pDevice != nullptr)
	{
		DIMOUSESTATE2 state{};//マウス情報//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			pMove->x = float(state.lX);
			pMove->y = float(state.lY);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputMouse::GetWheel(float* pWheel) const
{
	if (m_pDevice!=nullptr)
	{
		DIMOUSESTATE2 state{};//マウス情報//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			*pWheel = float(state.lZ);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//------------------------
// デバイス設定
//------------------------
HRESULT CInputDirectInputMouse::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// デバイス全体に設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16個のイベントを格納できるバッファを作成
	dipdw.dwData = 16;
	return m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
}

//-----------------------------
//
//dinputパッド
//
//-----------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputController::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(m_pDiInstance->guidInstance, &m_pDevice, nullptr)))return E_FAIL;

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// デバイス設定
	if (FAILED(SetProperty()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		// デバイスの破棄
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse);

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputController::Uninit(void)
{
	//エフェクトデバイスの破棄
	Vibrate(0);

	// デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// インプットの破棄
	CInputDirectInput::Unregister();
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetButton(span<bool> button) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE  state;// 入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			for (Index cntButton = 0; cntButton < button.size(); cntButton++)
			{
				button[cntButton] = (state.rgbButtons[0] & 0x80) != 0;
			}
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetStick(span<input::Axis> axis) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE  state;// 入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			axis[Left].x = float(state.lX) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Left].y = float(state.lY) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Right].x = float(state.lRx) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Right].y = float(state.lRy) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetTrigger(span<float> trigger) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE state{};//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			trigger[Left] = float(state.lZ) / float(DIRECTINPUT_TRIGGER_MAX);
			trigger[Left] = float(state.lRz) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------------------
//更新処理
//-------------------------
HRESULT CInputDirectInputController::GetSlider(span<float> slider) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE state{};//一時入力格納
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			// スライダー入力を取る
			slider[Left] = float(state.rglSlider[Left]) / float(DIRECTINPUT_TRIGGER_MAX);
			slider[Right] = float(state.rglSlider[Right]) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//アクセス権限の取得
		}
	}
	return E_FAIL;
}

//-------------
//振動
//-------------
void CInputDirectInputController::Vibrate(float leftMotorSpeed, float rightMotorSpeed)
{
	if (m_effect != nullptr)
	{// エフェクトを破棄
		m_effect->Stop();
		m_effect->Release();
		m_effect = nullptr;
	}

	if (leftMotorSpeed < 0.000001f) return; // 0の時は生成しない

	// フォースフィードバックのエフェクトを作成する
	DIEFFECT effect;
	ZeroMemory(&effect, sizeof(DIEFFECT));
	effect.dwSize = sizeof(DIEFFECT);
	effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	effect.dwDuration = INFINITE;  // 無限に続く振動
	effect.dwGain = DI_FFNOMINALMAX;
	effect.dwTriggerButton = DIEB_NOTRIGGER;

	// 軸と方向の設定
	DWORD rgdwAxes[1] = { DIJOFS_X };  // X軸の振動
	LONG rglDirection[1] = { 0 };      // 方向は正方向（0）
	effect.cAxes = 1;                  // 1軸
	effect.rgdwAxes = rgdwAxes;        // 使用する軸
	effect.rglDirection = rglDirection;// 振動の方向

	DICONSTANTFORCE cf;
	leftMotorSpeed = clamp(leftMotorSpeed, -1.0f, 1.0f);
	cf.lMagnitude = LONG(leftMotorSpeed * float(DI_FFNOMINALMAX));  // 振動の強さ (0 - DI_FFNOMINALMAX)

	effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	effect.lpvTypeSpecificParams = &cf;

	if (SUCCEEDED(m_pDevice->CreateEffect(GUID_ConstantForce, &effect, &m_effect, nullptr)))
	{// 作成できた
		if (FAILED(m_effect->Start(1, 0)))
		{// 失敗
			m_effect->Release();
			m_effect = nullptr;
		}
	}
}

//------------------------
// デバイス設定
//------------------------
HRESULT CInputDirectInputController::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// デバイス全体に設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16個のイベントを格納できるバッファを作成
	dipdw.dwData = 16;
	if (FAILED(m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))return E_FAIL;

	// デッドゾーンを20%に設定
	dipdw.dwData = 2000;
	if (FAILED(m_pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph)))return E_FAIL;

	// 軸オブジェクト設定
	return m_pDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS);
}

//------------------------
// 範囲設定コールバック
//------------------------
BOOL CALLBACK CInputDirectInputController::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	CInputDirectInputController* pThis = static_cast<CInputDirectInputController*>(pvRef);

	DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipr.diph.dwHow = DIPH_BYOFFSET;
	dipr.diph.dwObj = lpddoi->dwOfs;

	// dwOfsはDIJOYSTATE2構造体内のオフセット値なので、
	// C++標準のoffsetofを使って直接比較する
	switch (lpddoi->dwOfs)
	{
		// スティック系の軸 (中央がニュートラル)
	case offsetof(DIJOYSTATE2, lX):
	case offsetof(DIJOYSTATE2, lY):
	case offsetof(DIJOYSTATE2, lRx):
	case offsetof(DIJOYSTATE2, lRy):
		dipr.lMin = DIRECTINPUT_AXIS_MIN;
		dipr.lMax = DIRECTINPUT_AXIS_MAX;
		break;

		// トリガーやスライダー系の軸 (最小～最大)
	case offsetof(DIJOYSTATE2, lZ):
	case offsetof(DIJOYSTATE2, lRz):
	case offsetof(DIJOYSTATE2, rglSlider[0]): // DIJOFS_SLIDER(0) はこれに相当
	case offsetof(DIJOYSTATE2, rglSlider[1]): // DIJOFS_SLIDER(1) はこれに相当
	default: // 不明な軸もこちらで処理
		dipr.lMin = DIRECTINPUT_TRIGGER_MIN;
		dipr.lMax = DIRECTINPUT_TRIGGER_MAX;
		break;
	}

	HRESULT hr = pThis->m_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}
