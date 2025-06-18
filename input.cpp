//---------------------------------------
//
//入力処理[input.cpp]
//Author fuma sato
//
//---------------------------------------
#include <json/json.h>
#include"input.h"

#if 0

//----------------------------------
//
// インプットクラス (呼び出し用窓口)
//
//----------------------------------

// 静的メンバ変数の定義
CInput CInput::m_instance{}; // 自身のインスタンス

//-----------------
// 初期化
//-----------------
HRESULT CInput::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// キーボード
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboard;

		if (m_pKeyboard != nullptr)
		{
			if (FAILED(m_pKeyboard->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// マウス
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouse;

		if (m_pMouse != nullptr)
		{
			if (FAILED(m_pMouse->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// コントローラー
	if (m_pController == nullptr)
	{
		m_pController = new CInputController;

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
	if (m_pKeyboard != nullptr)
	{
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	// マウス
	if (m_pMouse != nullptr)
	{
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	// コントローラー
	if (m_pController != nullptr)
	{
		delete m_pController;
		m_pController = nullptr;
	}
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
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// コントローラー
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}
}

//-----------------
// 押している
//-----------------
bool CInput::IsPress(BUTTON button)
{
	return m_pKeyboard->IsPress(button);
}

//-----------------
// 押した瞬間
//-----------------
bool CInput::IsTrigger(BUTTON button)
{
	return m_pKeyboard->IsTrigger(button);
}

//-----------------
// 離した瞬間
//-----------------
bool CInput::IsRelease(BUTTON button)
{
	return m_pKeyboard->IsRelease(button);
}

//-----------------
// 長押し
//-----------------
bool CInput::IsRepeat(BUTTON button)
{
	return m_pKeyboard->IsRepeat(button);
}

//--------------------------
//
// キーボード
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// raw
	if (m_pRaw == nullptr)
	{
		m_pRaw = new CInputRawInputKeyboard;

		if (m_pRaw != nullptr)
		{
			if (SUCCEEDED(m_pRaw->Init(hInstanse, hWnd))) return S_OK;
		}
	}

	// direct
	if (m_pDirect == nullptr)
	{
		m_pDirect = new CInputDirectInputKeyboard;

		if (m_pDirect != nullptr)
		{
			if (SUCCEEDED(m_pDirect->Init(hInstanse, hWnd))) return S_OK;
		}
	}

	return E_FAIL;
}

//----------------------
//終了処理
//----------------------
void CInputKeyboard::Uninit(void)
{
	// direct
	if (m_pDirect != nullptr)
	{
		delete m_pDirect;
		m_pDirect = nullptr;
	}

	// raw
	if (m_pRaw != nullptr)
	{
		delete m_pRaw;
		m_pRaw = nullptr;
	}
}

//-------------------------
//更新処理
//-------------------------
void CInputKeyboard::Update(void)
{
	// raw
	if (m_pRaw != nullptr)
	{
		m_pRaw->Update();
	}

	// direct
	if (m_pDirect != nullptr)
	{
		m_pDirect->Update();
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputKeyboard::IsPress(CInput::BUTTON button)
{
	
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputKeyboard::IsTrigger(CInput::BUTTON button)
{
	
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputKeyboard::IsRelease(CInput::BUTTON button)
{
	
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputKeyboard::IsRepeat(CInput::BUTTON button)
{
	
}

//--------------------------
//
// マウス
//
//--------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// raw
	if (m_pRaw == nullptr)
	{
		m_pRaw = new CInputRawInputMouse;

		if (m_pRaw != nullptr)
		{
			if (SUCCEEDED(m_pRaw->Init(hInstanse, hWnd))) return S_OK;
		}
	}

	// direct
	if (m_pDirect == nullptr)
	{
		m_pDirect = new CInputDirectInputMouse;

		if (m_pDirect != nullptr)
		{
			if (SUCCEEDED(m_pDirect->Init(hInstanse, hWnd))) return S_OK;
		}
	}

	return E_FAIL;
}

//----------------------
//終了処理
//----------------------
void CInputMouse::Uninit(void)
{
	// direct
	if (m_pDirect != nullptr)
	{
		delete m_pDirect;
		m_pDirect = nullptr;
	}

	// raw
	if (m_pRaw != nullptr)
	{
		delete m_pRaw;
		m_pRaw = nullptr;
	}
}

//-------------------------
//更新処理
//-------------------------
void CInputMouse::Update(void)
{
	// raw
	if (m_pRaw != nullptr)
	{
		m_pRaw->Update();
	}

	// direct
	if (m_pDirect != nullptr)
	{
		m_pDirect->Update();
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputMouse::IsPress(CInput::BUTTON button)
{

}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputMouse::IsTrigger(CInput::BUTTON button)
{

}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputMouse::IsRelease(CInput::BUTTON button)
{

}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputMouse::IsRepeat(CInput::BUTTON button)
{

}

//--------------
//マウス移動
//--------------
CInputMouse::MouseMove CInputMouse::GetMove(void)
{

}

//----------------
//キー押下処理
//----------------
float CInputMouse::GetWheel(void)
{

}

//------------------------------------
//
// コントローラークラス
//
//------------------------------------

//-------------------
// 初期化
//-------------------
HRESULT CInputController::Init(HINSTANCE hInstanse, HWND hWnd)
{

}

//-------------------
// 初期化
//-------------------
void CInputController::Uninit(void)
{

}

//-------------------
// 初期化
//-------------------
void CInputController::Update(void)
{

}

//-------------------
// 初期化
//-------------------
bool CInputController::IsPress(CONTROLLER controller, CInput::BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
bool CInputController::IsTrigger(CONTROLLER controller, CInput::BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
bool CInputController::IsRelease(CONTROLLER controller, CInput::BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
bool CInputController::IsRepeat(CONTROLLER controller, CInput::BUTTON button)
{

}

//-------------------
// 初期化
//-------------------
void CInputController::SetVibrate(CONTROLLER controller, float fLeftPower, float fReghtPower)
{
	if (fLeftPower <= 0.0f && fReghtPower <= 0.0f)
	{
		if (XInputGetState(controller, &state) == ERROR_SUCCESS)
		{
			VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), Controller);
		}
		else
		{
			VibratedControllerStop(Controller);
		}
	}
	if (controller)
	{
		if (XInputGetState(controller, &state) == ERROR_SUCCESS)
		{
			VibrateController((WORD)(VIBRATION_MAX * fPower), (WORD)(VIBRATION_MAX * fPower), Controller);
		}
		else
		{
			VibratedControllerStop(Controller);
		}
	}
}

//-------------------
// 初期化
//-------------------
bool CInputController::IsXInputControllerConnected(CONTROLLER Controller)
{

}

//-------------------
// 初期化
//-------------------
bool CInputController::IsDirectInputControllerConnected(CONTROLLER Controller)
{

}

//-------------------
// 初期化
//-------------------
string_view CInputController::ControllerName(CONTROLLER Controller)
{

}

//-------------------
// 初期化
//-------------------
int CInputController::ControllerNum(void)
{

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
	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse, hWnd);

	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputRawInputKeyboard::Uninit(void)
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
void CInputRawInputKeyboard::Update(void)
{
	for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
	{
		m_KeyStateOld[nCntkey] = m_KeyState[nCntkey]; //前回のキー情報を保存
	}

	BYTE KeyState[MAX_KEY];//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			m_KeyState[nCntkey] = KeyState[nCntkey]; //一時的な情報を正規情報に渡す

			bool now = (m_KeyState[nCntkey] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_KeyStateOld[nCntkey] & 0x80) != 0; // 前回押していたかどうか

			m_bKeyPress[nCntkey] = now;           // 押
			m_bKeyTrigger[nCntkey] = now && !old; // 押下
			m_bKeyRelease[nCntkey] = (!now && old && m_KeyPressTime[nCntkey] >= CInput::RELEASE_TIME) ? true : false; // リリース

			if (m_bKeyTrigger[nCntkey] || m_bKeyRelease[nCntkey])
			{// キーの変化
				// タイムの初期化
				m_KeyPressTime[nCntkey] = 0.0f;
				m_KeyRepeatTime[nCntkey] = 0.0f;
			}
			else if (m_bKeyPress[nCntkey])
			{// 押している
				// タイムの増加
				m_KeyPressTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
				if (m_KeyPressTime[nCntkey] >= CInput::REPEAT_START_TIME)
				{// リピート中
					m_KeyRepeatTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
					if (m_KeyRepeatTime[nCntkey] >= CInput::REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_KeyRepeatTime[nCntkey] = 0.0f;
						m_bKeyRepeat[nCntkey] = now ? true : false;
					}
				}
			}
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputRawInputKeyboard::IsPress(size_t key)
{
	return m_bKeyPress[key];
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputRawInputKeyboard::IsTrigger(size_t key)
{
	return m_bKeyTrigger[key];
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputRawInputKeyboard::IsRelease(size_t key)
{
	return m_bKeyRelease[key];
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputRawInputKeyboard::IsRepeat(size_t key)
{
	return m_bKeyRepeat[key];
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
	// インプットオブジェクトの生成
	CInputDirectInput::Register(hInstanse, hWnd);

	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	m_pDevice->Acquire();//アクセス権限の取得

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputRawInputMouse::Uninit(void)
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
void CInputRawInputMouse::Update(void)
{
	m_MouseStateOld = m_MouseState; //前回のマウス情報を保存

	DIMOUSESTATE2 MouseState;//マウス情報//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_MouseState = MouseState;//一時的な情報を正規情報に渡す

		for (size_t nCntButton = 0; nCntButton < MAX; nCntButton++)
		{
			bool now = (m_MouseState.rgbButtons[nCntButton] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_MouseStateOld.rgbButtons[nCntButton] & 0x80) != 0; // 前回押していたかどうか

			m_bPress[nCntButton] = now;           // 押
			m_bTrigger[nCntButton] = now && !old; // 押下
			m_bRelease[nCntButton] = (!now && old && m_PressTime[nCntButton] >= RELEASE_TIME) ? true : false;

			if (m_bTrigger[nCntButton] || m_bRelease[nCntButton])
			{
				m_PressTime[nCntButton] = 0;
			}
			else if (m_bPress[nCntButton])
			{
				m_PressTime[nCntButton]++;
			}

			m_bRepeat[nCntButton] = (now && m_PressTime[nCntButton] >= REPEAT_TIME) ? true : false;

			m_Move[0] = (float)m_MouseState.lX;
			m_Move[1] = (float)m_MouseState.lY;
			m_Move[0] /= MOUSE_INA;
			m_Move[1] /= MOUSE_INA;

			m_WheelMove = (float)m_MouseState.lZ;
			m_WheelMove /= MOUSE_WHEEL_INA;
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputRawInputMouse::IsPress(BUTTON button)
{
	return m_bPress[button];
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputRawInputMouse::IsTrigger(BUTTON button)
{
	return m_bTrigger[button];
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputRawInputMouse::IsRelease(BUTTON button)
{
	return m_bRelease[button];
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputRawInputMouse::IsRepeat(BUTTON button)
{
	return m_bRepeat[button];
}

//--------------
//マウス移動
//--------------
CInputMouse::MouseMove CInputRawInputMouse::GetMove(void)
{
	return m_Move;
}

//----------------
//キー押下処理
//----------------
float CInputRawInputMouse::GetWheel(void)
{
	return m_WheelMove;
}

//--------------------------
//
//パッド
//
//--------------------------

//--------------------------------
//初期化処理
//--------------------------------
HRESULT CInputXInputController::Init(void)
{
	memset(&m_state, 0, sizeof(XINPUT_STATE));

	XInputEnable(true);

	return S_OK;
}

//------------------------
//終了処理
//------------------------
void CInputXInputController::Uninit(void)
{
	XInputEnable(false);
}

//------------------------
//更新処理
//------------------------
void CInputXInputController::Update(void)
{
	XINPUT_STATE state;

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		m_state = state;
	}
}

//--------------------------
//キー押下処理
//--------------------------
bool CInputXInputController::IsPress(JOYKEY key)
{
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (g_joykeyState[i].Gamepad.wButtons & (0x01 << key) || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				return true;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	return (g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key) || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}

//--------------------------
//キー入力処理
//--------------------------
bool CInputXInputController::IsTrigger(JOYKEY key)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = true;
				return true;
			}
			else if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = false;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = true;
		return true;
	}
	else if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = false;
		return false;
	}
	else
	{
		return false;
	}
}


//----------------------------
//キー離し処理
//----------------------------
bool CInputXInputController::IsRelease(JOYKEY key)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = false;
				if (KeyPutTime[i][key] >= RELEASE_TIME)
				{
					KeyPutTime[i][key] = 0;
					return true;
				}
				else
				{
					KeyPutTime[i][key] = 0;
				}
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				bOldJoy[i][key] = true;
			}
			else if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == false && Trigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = false;
		if (KeyPutTime[Controller][key] >= RELEASE_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			return true;
		}
		else
		{
			KeyPutTime[Controller][key] = 0;
			return false;
		}
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		bOldJoy[Controller][key] = true;
		return false;
	}
	else if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;
		return false;
	}
	else
	{
		return false;
	}
}

//----------------------------
//キー長押し処理
//----------------------------
bool CInputXInputController::IsRepeat(JOYKEY key)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				return true;
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldJoy[i][key] = true;
				}
			}
			else
			{
				bOldJoy[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		return true;
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldJoy[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldJoy[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
// 左スティック角度
//-----------------
float CInputXInputController::GetLeftStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// 左スティック角度
//-----------------
float CInputXInputController::GetRightStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// 左スティック倒し具合
//-----------------
float CInputXInputController::GetLeftStickLength(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// 左スティック倒し具合
//-----------------
float CInputXInputController::GetRightStickLength(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//----------------
//振動
//----------------
void CInputXInputController::Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)
{
	XINPUT_VIBRATION vibration{ wLeftMotorSpeed ,wRightMotorSpeed };
	XInputSetState(0, &vibration);
}

//----------------------------------
//
// DirectInputクラス
//
//----------------------------------

LPDIRECTINPUT8 CInputDirectInput::m_pInput{};
size_t CInputDirectInput::m_useCount{};

//-----------------
// 初期化
//-----------------
HRESULT CInputDirectInput::Register(HINSTANCE hInstanse, HWND hWnd)
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

	m_useCount++; // DirectInput参照数
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
		if (m_pInput != nullptr)
		{
			m_pInput->Release();
			m_pInput = nullptr;
		}
	}
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
	// DirectInput母体への登録
	CInputDirectInput::Register(hInstanse, hWnd);

	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権限の取得
	if (FAILED(m_pDevice->Acquire()))
	{
		return E_FAIL;
	}

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
void CInputDirectInputKeyboard::Update(void)
{
	for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
	{
		m_KeyStateOld[nCntkey] = m_KeyState[nCntkey]; //前回のキー情報を保存
	}

	BYTE KeyState[MAX_KEY];//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			m_KeyState[nCntkey] = KeyState[nCntkey]; //一時的な情報を正規情報に渡す

			bool now = (m_KeyState[nCntkey] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_KeyStateOld[nCntkey] & 0x80) != 0; // 前回押していたかどうか

			m_bKeyPress[nCntkey] = now;           // 押
			m_bKeyTrigger[nCntkey] = now && !old; // 押下
			m_bKeyRelease[nCntkey] = (!now && old && m_KeyPressTime[nCntkey] >= CInput::RELEASE_TIME) ? true : false; // リリース

			if (m_bKeyTrigger[nCntkey] || m_bKeyRelease[nCntkey])
			{// キーの変化
				// タイムの初期化
				m_KeyPressTime[nCntkey] = 0.0f;
				m_KeyRepeatTime[nCntkey] = 0.0f;
			}
			else if (m_bKeyPress[nCntkey])
			{// 押している
				// タイムの増加
				m_KeyPressTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
				if (m_KeyPressTime[nCntkey] >= CInput::REPEAT_START_TIME)
				{// リピート中
					m_KeyRepeatTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
					if (m_KeyRepeatTime[nCntkey] >= CInput::REPEAT_INTERVAL_TIME)
					{// リピート間隔
						m_KeyRepeatTime[nCntkey] = 0.0f;
						m_bKeyRepeat[nCntkey]= now ? true : false;
					}
				}
			}
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputDirectInputKeyboard::IsPress(size_t key)
{
	return m_bKeyPress[key];
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputDirectInputKeyboard::IsTrigger(size_t key)
{
	return m_bKeyTrigger[key];
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputDirectInputKeyboard::IsRelease(size_t key)
{
	return m_bKeyRelease[key];
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputDirectInputKeyboard::IsRepeat(size_t key)
{
	return m_bKeyRepeat[key];
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
	// インプットオブジェクトの生成
	CInputDirectInput::Register(hInstanse, hWnd);

	//インプットデバイスの作成
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}

	//フォーマット作成
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// バッファリングを有効にする
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	m_pDevice->Acquire();//アクセス権限の取得

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
void CInputDirectInputMouse::Update(void)
{
	m_MouseStateOld = m_MouseState; //前回のマウス情報を保存

	DIMOUSESTATE2 MouseState;//マウス情報//一時入力格納
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_MouseState = MouseState;//一時的な情報を正規情報に渡す

		for (size_t nCntButton = 0; nCntButton < MAX; nCntButton++)
		{
			bool now = (m_MouseState.rgbButtons[nCntButton] & 0x80) != 0;    // 今押しているかどうか
			bool old = (m_MouseStateOld.rgbButtons[nCntButton] & 0x80) != 0; // 前回押していたかどうか

			m_bPress[nCntButton] = now;           // 押
			m_bTrigger[nCntButton] = now && !old; // 押下
			m_bRelease[nCntButton] = (!now && old && m_PressTime[nCntButton] >= RELEASE_TIME) ? true : false;

			if (m_bTrigger[nCntButton] || m_bRelease[nCntButton])
			{
				m_PressTime[nCntButton] = 0;
			}
			else if (m_bPress[nCntButton])
			{
				m_PressTime[nCntButton]++;
			}

			m_bRepeat[nCntButton] = (now && m_PressTime[nCntButton] >= REPEAT_TIME) ? true : false;

			m_Move[0] = (float)m_MouseState.lX;
			m_Move[1] = (float)m_MouseState.lY;
			m_Move[0] /= MOUSE_INA;
			m_Move[1] /= MOUSE_INA;

			m_WheelMove = (float)m_MouseState.lZ;
			m_WheelMove /= MOUSE_WHEEL_INA;
		}
	}
	else
	{
		m_pDevice->Acquire();//アクセス権限の取得
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputDirectInputMouse::IsPress(BUTTON button)
{
	return m_bPress[button];
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputDirectInputMouse::IsTrigger(BUTTON button)
{
	return m_bTrigger[button];
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputDirectInputMouse::IsRelease(BUTTON button)
{
	return m_bRelease[button];
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputDirectInputMouse::IsRepeat(BUTTON button)
{
	return m_bRepeat[button];
}

//--------------
//マウス移動
//--------------
CInputMouse::MouseMove CInputDirectInputMouse::GetMove(void)
{
	return m_Move;
}

//----------------
//キー押下処理
//----------------
float CInputDirectInputMouse::GetWheel(void)
{
	return m_WheelMove;
}

//-----------------------------
//
//dinputパッド
//
//-----------------------------

//--------------------
//初期化処理
//--------------------
HRESULT CInputDirectInputController::Init(HINSTANCE hInstanse, HWND hWnd)
{
	//インプットオブジェクトの作成
	DirectInput8Create
	(
		hInstanse,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&g_pinput,
		nullptr
	);

	// DirectInputデバイスを列挙
	g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ATTACHEDONLY);

	return S_OK;
}

//----------------------
//終了処理
//----------------------
void CInputDirectInputController::Uninit(void)
{
	//エフェクトデバイスの破棄
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//キーボードデバイスの破棄
		if (g_DevdJoypad[i] != nullptr)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = nullptr;
		}
	}
	//インプットの破棄
	if (g_pinput != nullptr)
	{
		g_pinput->Release();
		g_pinput = nullptr;
	}
}

//-------------------------
//更新処理
//-------------------------
void CInputDirectInputController::Update(void)
{
	DIJOYSTATE  djoykeyState[CONTROLLER_MAX];//一時入力格納
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (g_DevdJoypad[i] != nullptr)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &djoykeyState[i])))
			{
				g_djoykeyState[i] = djoykeyState[i];//一時的な情報を正規情報に渡す
			}
			else
			{
				ControllerNum++;
			}
		}
		else
		{
			ControllerNum++;
		}
	}

	if (g_DcontrollerNum!=(CONTROLLER_MAX-ControllerNum))
	{
		SetUnCon(true);
		InputReSet();
		// DirectInputデバイスを列挙
		g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ATTACHEDONLY);
	}
}

//------------------------------
//キー押下処理
//------------------------------
bool CInputDirectInputController::IsPress(int key)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false)
			{
				return true;
			}
		}

		return false;
	}

	return (g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false;
}

//------------------------------
//キー入力時の処理
//------------------------------
bool CInputDirectInputController::IsTrigger(int key)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][key] == false && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				bOldkey[i][key] = true;
				return true;
			}
			else if (bOldkey[i][key] == true && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == false)
			{
				bOldkey[i][key] = false;
			}
		}

		return false;
	}

	if (bOldkey[Controller][key] == false && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		bOldkey[Controller][key] = true;
		return true;
	}
	else if (bOldkey[Controller][key] == true && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == false)
	{
		bOldkey[Controller][key] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー話離した際の処理
//------------------------------
bool CInputDirectInputController::IsRelease(int key)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][key] == true && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == false)
			{
				bOldkey[i][key] = false;
				if (KeyPutTime[i][key] >= RELEASE_TIME)
				{
					KeyPutTime[i][key] = 0;
					return true;
				}
				else
				{
					KeyPutTime[i][key] = 0;
				}
			}
			else if (bOldkey[i][key] == false && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				bOldkey[i][key] = true;
			}
			else if (bOldkey[i][key] == true && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][key]++;
			}
		}

		return false;
	}

	if (bOldkey[Controller][key] == true && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == false)
	{
		bOldkey[Controller][key] = false;
		if (KeyPutTime[Controller][key] >= RELEASE_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			return true;
		}
		else
		{
			KeyPutTime[Controller][key] = 0;
			return false;
		}
	}
	else if (bOldkey[Controller][key] == false && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		bOldkey[Controller][key] = true;
		return false;
	}
	else if (bOldkey[Controller][key] == true && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][key]++;
		return false;
	}
	else
	{
		return false;
	}
}

//------------------------------
//キー長押し処理
//------------------------------
bool CInputDirectInputController::IsRepeat(int key)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][key] == true && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				return true;
			}
			else if (bOldkey[i][key] == false && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldkey[i][key] = true;
				}
			}
			else
			{
				bOldkey[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (bOldkey[Controller][key] == true && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[Controller][key] == false && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldkey[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldkey[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
//スティック処理
//-----------------
float CInputDirectInputController::GetLeftStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_djoykeyState[Controller].lX;
		fStick[1] = (float)g_djoykeyState[Controller].lY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_djoykeyState[Controller].lZ;
		fStick[1] = (float)g_djoykeyState[Controller].lRz;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	fStick[0] += DSTICK_NUM;
	fStick[1] += DSTICK_NUM;

	return &fStick[0];
}

//----------------------
//スライダー軸
//----------------------
float CInputDirectInputController::GetSlider(int nSlider)
{
	float fSlider = 0.0f;

	fSlider = (float)g_djoykeyState[Controller].rglSlider[nSlider];

	fSlider /= STICK_NUM;

	fSlider += DSTICK_NUM;

	return fSlider;
}

//------------------------
//ハットスイッチ
//------------------------
bool CInputDirectInputController::IsPressUp(int nPov)
{
	float fPov = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (fPov==fData)
			{
				return true;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	return (fPov == fData) ? true : false;
}

//-------------------------
//
//-------------------------
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//-----------------------
//
//-----------------------
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return true;
				}
				else
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return false;
				}
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return false;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return true;
		}
		else
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return false;
		}
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return false;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;
		return false;
	}
	else
	{
		return false;
	}
}

//---------------------
//
//---------------------
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;

				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				}
				return false;
			}
			else
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
			}


		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;

		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		}
		return false;
	}
	else
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
		return false;
	}
}

//-------------
//振動
//-------------
void CInputDirectInputController::Vibrate(LONG lMotorPower)
{
	VibrateStop(); // 停止

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
	cf.lMagnitude = lMotorPower;  // 振動の強さ (0 - DI_FFNOMINALMAX)

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

//---------
//振動
//---------
void CInputDirectInputController::VibrateStop(void)
{
	if (m_effect != nullptr)
	{
		m_effect->Stop();
		m_effect->Release();
		m_effect = nullptr;
	}
}

//--------------------------------
//接続確認
//--------------------------------

//-----------
//Xinput
//-----------
bool IsXInputControllerConnected(CONTROLLER Controller)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(Controller, &joykeyState[Controller]) == ERROR_SUCCESS;
}

//------------
//dinput
//------------
bool IsDirectInputControllerConnected(CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_DevdJoypad[i] != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	return g_DevdJoypad[Controller] != nullptr;
}

//------------------
//コントローラー名
//------------------
char *ControllerName(CONTROLLER Controller)
{
	return &g_ConName[Controller][0];
}

//------------------
//コントローラー数
//------------------
int ControllerNum(CONTYPE Contype)
{
	if (Contype==CONTYPE_X)
	{
		return g_XcontrollerNum;
	}
	else
	{
		return g_DcontrollerNum;
	}
}

//---------------------------
//デバイス情報取得
//---------------------------
BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	static int nControllerNum = 0;
	static int XcontrollerNum = 0;

	if (g_bUnCon)
	{
		nControllerNum = 0;
		XcontrollerNum = 0;
		g_DcontrollerNum = 0;
		g_XcontrollerNum = 0;
		g_bX = false;
		g_bUnCon = false;
		//エフェクトデバイスの破棄
		VibratedControllerStop(CONTROLLER_MAX);
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			//キーボードデバイスの破棄
			if (g_DevdJoypad[i] != nullptr)
			{
				g_DevdJoypad[i]->Unacquire();
				g_DevdJoypad[i]->Release();
				g_DevdJoypad[i] = nullptr;
			}
		}
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			g_ConName[i][0] = '\0';
		}
	}

	if ((nControllerNum+XcontrollerNum)<=CONTROLLER_MAX)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		g_pDiInstance[nControllerNum] = pdidInstance;

		if (XInputGetState(XcontrollerNum, &state) == ERROR_SUCCESS)
		{
			if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[XcontrollerNum], nullptr)))
			{//インプットデバイスの作成
				HINSTANCE hInstanse;
				HWND hWnd;

				hInstanse = GethInstanse();
				hWnd = GethWnd();

				//フォーマット作成
				g_DevdJoypad[nControllerNum]->SetDataFormat(&c_dfDIJoystick);

				g_DevdJoypad[nControllerNum]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

				g_DevdJoypad[nControllerNum]->Acquire();//アクセス権限の取得

				strcpy(&g_ConName[nControllerNum][0], pdidInstance->tszProductName);
				XcontrollerNum++;
			}
		}
		else if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[nControllerNum + XNum()], nullptr)))
		{//インプットデバイスの作成
			HINSTANCE hInstanse;
			HWND hWnd;

			hInstanse = GethInstanse();
			hWnd = GethWnd();

			//フォーマット作成
			g_DevdJoypad[nControllerNum + XNum()]->SetDataFormat(&c_dfDIJoystick);

			g_DevdJoypad[nControllerNum + XNum()]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

			g_DevdJoypad[nControllerNum + XNum()]->Acquire();//アクセス権限の取得

			strcpy(&g_ConName[nControllerNum + XNum()][0], pdidInstance->tszProductName);
			nControllerNum++;
		}
	}
	g_DcontrollerNum = nControllerNum+XcontrollerNum;

	// 列挙を続行
	return DIENUM_CONTINUE;
}

//----------
//
//----------
int XNum(void)
{
	int XNum = 0;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			XNum++;
		}
	}
	return XNum;
}

LPDIRECTINPUT8 GetInput(void)
{
	return g_pinput;
}

void SetUnCon(bool bUnCon)
{
	g_bUnCon = bUnCon;
}

//-----------------------
//コントローラー管理初期化
//-----------------------
void InputReSet(void)
{
	g_bUnCon = false;
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		g_ConName[i][0] = '\0';
	}
	g_XcontrollerNum = 0;
	g_DcontrollerNum = 0;
    g_bX = false;
	//エフェクトデバイスの破棄
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//キーボードデバイスの破棄
		if (g_DevdJoypad[i] != nullptr)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = nullptr;
		}
	}
}

//------------------------------------------------
//コントローラー情報取得
//------------------------------------------------
const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller)
{
	return g_pDiInstance[Controller];
}

//----------------------------------------------
//Xinputコントローラー情報取得
//----------------------------------------------
XINPUT_STATE GetXstate(CONTROLLER Controller)
{
	return g_joykeyState[Controller];
}

//----------------------------------------------
//dinputコントローラー情報取得
//----------------------------------------------
DIJOYSTATE Getdstate(CONTROLLER Controller)
{
	return g_djoykeyState[Controller];
}

#endif