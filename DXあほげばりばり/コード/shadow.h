//=============================================================================
//
// 地面処理 [field.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
typedef struct
{
	float nPosx;//弾の座標
	float nPosz;    
			
} SHADOWDATA;

#define SHADOW_MAX (20)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int GetBulletFlag(int nNum);
D3DXVECTOR3 GetBulletPos(int nNum);
#endif
