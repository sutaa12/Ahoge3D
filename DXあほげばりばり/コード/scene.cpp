//=============================================================================
//
// シーン遷移 [scene.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "scene.h"
#include "feed.h"
#include "gamemain.h"
#include "titlemain.h"
#include "resultmain.h"
#include "rankingmain.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FEED_SPEED (0.05f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitNextSecen(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
//初期化するモード
MODE nInitMode;
//選択するモード
MODE nUpdateMode;
//描画するモード
MODE nDrawMode;
//終了するモード
MODE nUninitMode;

//フェード値
float g_fFeed = 1.0f;
//*****************************************************************************
// シーン遷移用関数ポインタ
//*****************************************************************************
HRESULT (*ModeInit[MODE_MAX])(void)
=
{
	InitTitle,
	InitGameMain,
	InitResult,
	RankingInit,
	NULL,
	NULL
};

void (*ModeUpdate[MODE_MAX])(void)
=
{
	UpdateTitle,
	UpdateGameMain,
	UpdateResult,
	RankingUpdate,
	NULL,
	NULL
};

void (*ModeDraw[MODE_MAX])(void)
=
{
	DrawTitle,
	DrawGameMain,
	DrawResult,
	RankingDraw,
	NULL,
	NULL
};

void (*ModeUninit[MODE_MAX])(void)
=
{
	UninitTitle,
	UninitGameMain,
	UninitResult,
	RankingUninit,
	NULL,
	NULL
};

//*****************************************************************************
// シーン初期化
//*****************************************************************************
void InitScene(void)
{
	//ゲームモード初期化
	nUpdateMode = MODE_FADE;
	//次に初期化するモード
	nInitMode = MODE_TITLE;
	nUninitMode = MODE_NONE;
	nDrawMode = MODE_NONE;
	//フェード設定
	g_fFeed = 1;
	//フェード初期化
	InitFeed();
}
//*****************************************************************************
// シーン解放
//*****************************************************************************
void UninitScene(void)
{
	//アップデートが殻でなくフェードでもないなら
	if(nUpdateMode != MODE_NONE && nUpdateMode != MODE_FADE)
	{
	//シーン解放
	(ModeUninit[nUpdateMode])();
	}
	//フェード解放
	UninitFeed();
}
//*****************************************************************************
// シーン更新
//*****************************************************************************
void UpdateScene(void)
{
	//初期化に入ってなくアップデートに入っていてフェードでなく終了もない場合
	if(nInitMode == MODE_NONE && (nUpdateMode != MODE_NONE) && (nUpdateMode != MODE_FADE) &&	 nUninitMode == MODE_NONE)
	{
		(*ModeUpdate[nUpdateMode])();//アップデート関数呼び出し
	}
	//フェード処理
	if(nUpdateMode == MODE_FADE)
	{
		g_fFeed += FEED_SPEED;
		//フェードが1を超えたら
		if(g_fFeed > 1)
		{
			//終了処理が空でない場合
			if(ModeUninit[nUninitMode] !=NULL)
			{
				(*ModeUninit[nUninitMode])();
			}
			//初期化処理が空でない場合
			if(ModeInit[nInitMode] != NULL)
			{
				(*ModeInit[nInitMode])();
				//更新のシーン番号を初期化のに入れる
				nUpdateMode = nInitMode;
				//描画もセット
				nDrawMode = nInitMode;
				//初期化と終了を空に
				nInitMode = MODE_NONE;
				nUninitMode = MODE_NONE;
			}
		}
		
	}else{
			//フェードアウト
			if(g_fFeed > 0)
			{
				g_fFeed -= FEED_SPEED;
			}
			else
			//フェードが0以下なら０にする
			if(g_fFeed < 0)
			{
				g_fFeed = 0;
			}
	}
	//フェードが1以上なら1にする
	if(g_fFeed > 1)
	{
		g_fFeed = 1;
	}
	//フェード更新
	UpdateFeed(g_fFeed);
}
//*****************************************************************************
// シーン描画
//*****************************************************************************
void DrawScene(void)
{
	if(nDrawMode != MODE_NONE)
	{
	//描画
	(ModeDraw[nDrawMode])();
	}
	//フェード描画
	DrawFeed();
}
//*****************************************************************************
// 次のシーン準備	
//*****************************************************************************
void SetNextScene(MODE nMode)
{
	//空のモードでなければ
	if(nMode !=	MODE_NONE)
	{
		//解放にアップデートを入れる
		nUninitMode =	 nUpdateMode;
		//フェードに変更
		nUpdateMode = MODE_FADE;
		//初期化セット
		nInitMode = nMode;
	}
}
//*****************************************************************************
// シーン初期化
//*****************************************************************************
void InitNextScene(void)
{
	//初期化処理
	(*ModeInit[nInitMode])();
}
	
//End Of File