//=============================================================================
//
// �n�ʏ��� [field.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
typedef struct
{
	float nPosx;//�e�̍��W
	float nPosz;    
			
} SHADOWDATA;

#define SHADOW_MAX (20)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int GetBulletFlag(int nNum);
D3DXVECTOR3 GetBulletPos(int nNum);
#endif
