//=============================================================================
//
// 入力処理 [input.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_KEY (256)//キーの最大数
#define START_RE (40)//リピート開始
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pDInput=NULL;//DirectInputオブジェクトポインタ
LPDIRECTINPUTDEVICE8 g_pDIDevkeyboard=NULL;//デバイス入力のポインタ(キーボード用)

BYTE g_akeyState[MAX_KEY];//キーボードの状態格納
BYTE g_keyTrigger[MAX_KEY];
BYTE g_keyRelease[MAX_KEY];
BYTE g_keyRepeat[MAX_KEY];
int g_nReCnt[MAX_KEY];//リピートのカウント


//============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;
	if(g_pDInput==NULL)//本体ができてない場合
	{
		// DirectInputオブジェクトの作成
		hr=DirectInput8Create	(hInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void**)&g_pDInput,
								NULL);
		if(FAILED(hr))
		{
			//メッセージボックス表示
			MessageBox(NULL,"作成失敗","DI作成失敗",MB_OK);
			return hr;
		}
	}
	// デバイスオブジェクトを作成
	hr=g_pDInput->CreateDevice	(GUID_SysKeyboard,
								&g_pDIDevkeyboard,
								NULL);
		if(FAILED(hr))
		{
			//メッセージボックス表示
			MessageBox(NULL,"作成失敗","CD作成失敗",MB_OK);
			return hr;
		}


	// データフォーマットを設定
	hr=g_pDIDevkeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr))
		{
			//メッセージボックス表示
			MessageBox(NULL,"作成失敗","SDF作成失敗",MB_OK);
			return hr;
		}


	// 協調モードを設定（フォアグラウンド＆非排他モード）
		hr=g_pDIDevkeyboard->SetCooperativeLevel	(hWnd,
													(DISCL_FOREGROUND|DISCL_NONEXCLUSIVE));
		if(FAILED(hr))
		{
			//メッセージボックス表示
			MessageBox(NULL,"作成失敗","SCL作成失敗",MB_OK);
			return hr;
		}
		
		for(int nCnt=0;nCnt<MAX_KEY;nCnt++)
		{
			g_nReCnt[nCnt]=0;
		}

	// キーボードへのアクセス権を獲得(入力制御開始)

		g_pDIDevkeyboard->Acquire();
		
	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	// DirectInputオブジェクトの開放
	if(g_pDInput!=NULL)
	{
		g_pDInput->Release();
		g_pDInput=NULL;
	}

	// デバイスオブジェクトの開放

	if(g_pDIDevkeyboard!=NULL)
	{
		g_pDIDevkeyboard->Unacquire();
		g_pDIDevkeyboard->Release();
		g_pDIDevkeyboard=NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE akeyState[MAX_KEY];//キーボードの格納状態

	if(SUCCEEDED(g_pDIDevkeyboard->GetDeviceState(sizeof(akeyState),&akeyState[0])))
	{
		for(int nCnt=0;nCnt<MAX_KEY;nCnt++)
		{

			g_keyRelease[nCnt]=(akeyState[nCnt]^g_akeyState[nCnt])&g_akeyState[nCnt];//リリース情報作成
			
			//リピート情報作成
			if(g_nReCnt[nCnt]<START_RE)//開始以下なら
			{
				g_keyRepeat[nCnt]=g_keyTrigger[nCnt];//トリガーに
							if(g_nReCnt[nCnt]>0)//開始以下なら
							{
								g_nReCnt[nCnt]++;
							}
			}else{//開始以上なら
				g_keyRepeat[nCnt]=akeyState[nCnt];//トリガーに
			}


			g_keyTrigger[nCnt]=(akeyState[nCnt]^g_akeyState[nCnt])&~g_akeyState[nCnt];//トリガー情報作成
			g_akeyState[nCnt]=akeyState[nCnt];//キー情報更新
		}
	}else
	{
		g_pDIDevkeyboard->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return(g_akeyState[nKey] & 0x80) ? TRUE : FALSE;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{

	return(g_keyTrigger[nKey] & 0x80) ? TRUE : FALSE;//トリガーの情報と一致するか
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return(g_keyRepeat[nKey] & 0x80) ? TRUE : FALSE;

}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return(g_keyRelease[nKey] & 0x80) ? TRUE : FALSE;
}


void RepeatSet(int nKey)
{
	g_nReCnt[nKey]++;
}
void RepaetReset(int nKey)
{g_nReCnt[nKey]=0;
}