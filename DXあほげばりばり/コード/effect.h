//=============================================================================
//
// 弾処理 [Effect.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EF_SIZEWH (20)//爆発の画像サイズ
#define EF_SIZESPEED (0.3f)//爆発の最大サイズ
#define EF_ALPHASPEED (0.05f)//爆発の表示時間
#define EF_MAX (500)//爆発の最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	float nPosx;//弾の座標
	float nPosy;    
	float nPosz;
	float fSizex;//大きさ
	float fSizey;
	float fSizez;
	float fAlpha;//透明度
	float nRot;//弾のむき
	int nCnt;//フレームカウント
	int nFlag;//弾のフラグ
	D3DCOLOR dCol;//色

}EFFECTDATA;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(float x,float y,float z,float rot,D3DCOLOR col);

#endif
