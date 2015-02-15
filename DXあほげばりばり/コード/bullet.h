//=============================================================================
//
// 弾処理 [Bullet.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define B_SIZEWH (10)
#define B_MAX (20)
#define B_SPEED (5)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	float fPosx;//弾の座標
	float fPosy;    
	float fPosz;
	float nRot;//弾のむき
	int nFlag;//弾のフラグ
	D3DCOLOR Col;//弾のエフェクト色 
	
	PosCircle fPosCicle;//中心座標と半径のデータ
	MtxSet mtxset;//マトリックスデータ格納
		
} BULLETDATA;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetShot(float x,float y,float z,float rot);

void SetEnemyLife(int nNum);
PosCircle GetEnemyPos(int nNum);
#endif
