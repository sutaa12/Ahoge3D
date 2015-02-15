//=============================================================================
//
// メッシュドームの処理 [meshdoom.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MESHDOOM_H_
#define _MESHDOOM_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshDoom(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshDoom(void);
void UpdateMeshDoom(void);
void DrawMeshDoom(void);

#endif
