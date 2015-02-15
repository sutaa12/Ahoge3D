//=============================================================================
//
// タイトル処理 [titlemain.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include"light.h"
#include "titlemain.h"
#include "titlelogo.h"
#include "scene.h"
#include "titlecamera.h"
#include "titleplayer.h"
#include "titleenemy.h"
#include "input.h"
#include "sound.h"
#include "titlestart.h"
#include "meshdoom.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_TitleCOUNT;
//*****************************************************************************
//初期化処理 
//*****************************************************************************
HRESULT  InitTitle(void)
{
	InitLight();
	InitTitleCamera();
	InitTitlePlayer();
	InitMeshDoom(20,20,10000,10000);
	InitTitleStart();
	//背景初期化
	if(FAILED(InitTitleLogo()))
	{
		return E_FAIL;
	}
	PlaySound(SOUND_LABEL_BGM000);
	g_TitleCOUNT = 0;
	return S_OK;
}
//*****************************************************************************
//終了処理 
//*****************************************************************************
void UninitTitle(void)
{
	UninitLight();
	UninitTitlePlayer();
	//背景解放
	UninitTitleLogo();
	UninitTitleStart();
	UninitMeshDoom();
	StopSound();
}
//*****************************************************************************
//更新処理 
//*****************************************************************************
void UpdateTitle(void)
{
	UpdateLight();
	UpdateTitlePlayer();
	UpdateTitleCamera();
	UpdateTitleStart();
	UpdateMeshDoom();
	//背景更新
	UpdateTitleLogo();
	//PrintMessageProc("あほ毛バリバリのDX版のようなもの\n");

	PrintMessageProc("\n\n\n\n\n\n\n\n\nWASDで移動\nQEで視点回転\nSPACE押しっぱなしで\n近くの敵に引き寄せ\n離すと放電\n敵と5回当たると終了\n敵により近い所で放電すると多くスコアが入る！\n");
	//PrintMessageProc("ENTERでスタート\n");
	//ボタンでシーン切り替え
	if(GetKeyboardTrigger(DIK_RETURN))
	{
		SetNextScene(MODE_GAMEMAIN);
	}
	if(g_TitleCOUNT  > 30 * 60)
	{
		SetNextScene(MODE_RANKING);
	}
	g_TitleCOUNT++;
}
//*****************************************************************************
//描画処理 
//*****************************************************************************
void DrawTitle(void)
{
	DrawMeshDoom();
	SetTitleCamera();
	DrawTitlePlayer();

	DrawTitleStart();	
		//背景描画
	DrawTitleLogo();
}
