//=============================================================================
//
// �e���� [Bullet.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define B_SIZEWH (10)
#define B_MAX (20)
#define B_SPEED (5)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	float fPosx;//�e�̍��W
	float fPosy;    
	float fPosz;
	float nRot;//�e�̂ނ�
	int nFlag;//�e�̃t���O
	D3DCOLOR Col;//�e�̃G�t�F�N�g�F 
	
	PosCircle fPosCicle;//���S���W�Ɣ��a�̃f�[�^
	MtxSet mtxset;//�}�g���b�N�X�f�[�^�i�[
		
} BULLETDATA;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetShot(float x,float y,float z,float rot);

void SetEnemyLife(int nNum);
PosCircle GetEnemyPos(int nNum);
#endif
