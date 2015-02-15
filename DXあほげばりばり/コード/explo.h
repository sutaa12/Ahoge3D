//=============================================================================
//
// 弾処理 [Explo.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _EXPLO_H_
#define _EXPLO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define E_SIZEWH (15)//爆発の画像サイズ
#define E_SIZESPEED (0.8f)//爆発の最大サイズ
#define E_ALPHASPEED (0.07f)//爆発の表示時間
#define E_MAX (20)//爆発の最大数
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
			
}EXPLODATA;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitExplo(void);
void UninitExplo(void);
void UpdateExplo(void);
void DrawExplo(void);
void SetExplo(float x,float y,float z,float rot);

#endif
