//=============================================================================
//
// カメラ処理 [camera.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void PoseCameUpdate(void);
void SetCamera(void);
D3DXVECTOR3 GetRotCamera(void);
D3DXVECTOR3 GetCameraP(void);
D3DXVECTOR3 GetCameraR(void);

D3DXVECTOR3 PlayerRot(void);
D3DXVECTOR3 GetPlayerPosition(void);
D3DXVECTOR3 GetPlayerSpeed(void);
#endif
