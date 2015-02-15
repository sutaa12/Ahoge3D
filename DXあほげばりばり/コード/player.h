//=============================================================================
//
// �v���C���[���� [player.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
typedef struct
{
	PosCircle fPosCicle;//���S���W�Ɣ��a�̃f�[�^
	MtxSet mtxset;//�}�g���b�N�X�f�[�^�i�[
	int AttackFlag;//�U���t���O
	float fINRad;	//�������ʒu
	float fOUTRad;//������΂��\����
	bool bHit;//���G���ǂ���
	bool bPose;//�|�[�Y���ǂ���
		
}PLAYERDATA;
//�}�N����`
typedef enum
{
	P_ACT_WAIT = 0,
	P_ACT_IN,
	P_ACT_OUT,
	P_ACT_MAX
}PLAYER_ACT;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void FixPI(float fRot);
int GetPlayerFlag(void);

bool GetPosePlayer(void);
void SetPosePlayer(bool bChoice);

D3DXVECTOR3 GetPlayerPosition(void);
D3DXVECTOR3 PlayerRot(void);
D3DXVECTOR3 GetField(void);

#endif
