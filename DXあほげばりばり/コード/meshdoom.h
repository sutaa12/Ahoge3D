//=============================================================================
//
// ���b�V���h�[���̏��� [meshdoom.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MESHDOOM_H_
#define _MESHDOOM_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshDoom(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshDoom(void);
void UpdateMeshDoom(void);
void DrawMeshDoom(void);

#endif
