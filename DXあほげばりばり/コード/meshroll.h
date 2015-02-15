//=============================================================================
//
// メッシュ円柱の処理 [meshroll.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MESHROLL_H_
#define _MESHROLL_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshRoll(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshRoll(void);
void UpdateMeshRoll(void);
void DrawMeshRoll(void);

#endif
