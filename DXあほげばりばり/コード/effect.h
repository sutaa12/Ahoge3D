//=============================================================================
//
// �e���� [Effect.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EF_SIZEWH (20)//�����̉摜�T�C�Y
#define EF_SIZESPEED (0.3f)//�����̍ő�T�C�Y
#define EF_ALPHASPEED (0.05f)//�����̕\������
#define EF_MAX (500)//�����̍ő吔
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
	D3DCOLOR dCol;//�F

}EFFECTDATA;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(float x,float y,float z,float rot,D3DCOLOR col);

#endif
