//=============================================================================
//
// �e���� [Explo.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _EXPLO_H_
#define _EXPLO_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define E_SIZEWH (15)//�����̉摜�T�C�Y
#define E_SIZESPEED (0.8f)//�����̍ő�T�C�Y
#define E_ALPHASPEED (0.07f)//�����̕\������
#define E_MAX (20)//�����̍ő吔
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	float nPosx;//�e�̍��W
	float nPosy;    
	float nPosz;
	float fSizex;//�傫��
	float fSizey;
	float fSizez;
	float fAlpha;//�����x
	float nRot;//�e�̂ނ�
	int nCnt;//�t���[���J�E���g
	int nFlag;//�e�̃t���O
			
}EXPLODATA;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitExplo(void);
void UninitExplo(void);
void UpdateExplo(void);
void DrawExplo(void);
void SetExplo(float x,float y,float z,float rot);

#endif
