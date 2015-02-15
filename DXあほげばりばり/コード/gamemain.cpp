//=============================================================================
//
// ゲーム中処理 [gamemain.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================

#include "gamemain.h"
#include "sound.h"
#include "scene.h"
#include "player.h"
#include "input.h"
#include "light.h"
#include "meshfield.h"
#include "shadow.h"
#include "camera.h"
#include "ball.h"
#include "bullet.h"
#include "wall.h"
#include "explo.h"
#include "score.h"
#include "effect.h"
#include "meshroll.h"
#include "meshdoom.h"
#include "enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
PosCircle GetPlayer(void);
void SetPlayerFlag(int nLife);
//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
//初期化処理 
//*****************************************************************************
HRESULT InitGameMain(void)
{
	PlaySound(SOUND_LABEL_BGM001);
	//ライト初期化
	if(FAILED(InitLight()))
	{
		return E_FAIL;
	}

	// モデルの初期化処理
	if(FAILED(InitPlayer()))
	{
		return E_FAIL;
	}
	// 敵の初期化処理
	if(FAILED(InitEnemy()))
	{
		return E_FAIL;
	}
	//メッシュフィールド初期化
	if(FAILED(InitMeshField(10,10,FIELDSIZEMAX,FIELDSIZEMAX)))
	{
		return E_FAIL;
	}
	//影初期化
	if(FAILED(InitShadow()))
	{
		return E_FAIL;
	}
	//ボール初期化
	if(FAILED(InitBall()))
	{
		return E_FAIL;
	}
	//メッシュウォール初期化
	if(FAILED(InitWall()))
	{
		return E_FAIL;
	}
	//弾初期化
	if(FAILED(InitBullet()))
	{
		return E_FAIL;
	}
	//爆発初期化
	if(FAILED(InitExplo()))
	{
		return E_FAIL;
	}
	//エフェクト初期化
	if(FAILED(InitEffect()))
	{
		return E_FAIL;
	}
	//スコア初期化
	if(FAILED(InitScore()))
	{
		return E_FAIL;
	}
	//円柱初期化
	if(FAILED(InitMeshRoll(12,1,150,50)))
	{
		return E_FAIL;
	}
	//ドーム初期化
	if(FAILED(InitMeshDoom(12,12,FIELDSIZEMAX*100,FIELDSIZEMAX*100)))
	{
		return E_FAIL;
	}
		//カメラライト初期化
	if(FAILED(InitCamera()))
	{
		return E_FAIL;
	}
	return S_OK;

}
//*****************************************************************************
//終了処理 
//*****************************************************************************
void UninitGameMain(void)
{
	StopSound();
	//カメラ解放
	UninitCamera();
	//ライト解放
	UninitLight();
	// 影の解放
	UninitShadow();
	//モデルの解放
	UninitPlayer();
	//メッシュフィールド解放
	UninitMeshField();
	//ボール解放
	UninitBall();
	//メッシュウォール解放
	UninitWall();
	//弾解放
	UninitBullet();
	//爆発解放
	UninitExplo();
	//エフェクト解放
	UninitEffect();
	//スコア解放
	UninitScore();
	//円柱解放
	UninitMeshRoll();
	//ドーム解放
	UninitMeshDoom();
	//敵解放
	UninitEnemy();
}
//*****************************************************************************
//更新処理 
//*****************************************************************************
void UpdateGameMain(void)
{
	if(GetPosePlayer() != 1)
	{
		//カメラ更新
		UpdateCamera();
		//ライト更新
		UpdateLight();
		//円柱更新
		UpdateMeshRoll();
		//ドーム更新
		UpdateMeshDoom();
		// モデルの更新処理
		UpdatePlayer();
		//メッシュフィールド更新
		UpdateMeshField();
		//影更新
		UpdateShadow();
		//ボール更新
		UpdateBall();
		//メッシュウォール更新
		UpdateWall();
		//弾更新
		UpdateBullet();
		//爆発更新
		UpdateExplo();
		//エフェクト更新
		UpdateEffect();
		//スコア更新
		UpdateScore();
		//敵更新
		UpdateEnemy();
	}else{
#if _DEBUG
	PoseCameUpdate();
	if(GetKeyboardTrigger(DIK_P))
	{
		SetPosePlayer(false);
	}

#else
		PrintMessageProc("Pで再開\nEnterでタイトルへ\n");
		PrintMessageProc("WASDで移動\nQEで視点回転\nSPACE押しっぱなしで近くの敵に引き寄せ\n離すと放電\n敵と5回当たると終了\n敵により近い所で放電すると多くスコアが入る！\n");
		if(GetKeyboardTrigger(DIK_P))
		{
			SetPosePlayer(false);
		}else
		if(GetKeyboardTrigger(DIK_RETURN))
		{
			SetNextScene(MODE_TITLE);
		}
#endif
	}
}
//*****************************************************************************
//描画処理 
//*****************************************************************************
void DrawGameMain(void)
{
		//カメラセット
		SetCamera();
		//円柱描画
		DrawMeshRoll();
		//ドーム描画
		DrawMeshDoom();


				//メッシュフィールド描画
		DrawMeshField();
		//メッシュウォール描画
		DrawWall();

				//影描画
		DrawShadow();
		// モデルの描画処理
		DrawPlayer();
		//弾描画
		DrawBullet();


		//スコア描画
		DrawScore();
		//敵描画
		DrawEnemy();
				//エフェクト描画
		DrawEffect();
				//爆発描画
		DrawExplo();
}

//End Of File