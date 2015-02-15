//=============================================================================
//
// メッシュ地面の処理 [meshfield.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif
