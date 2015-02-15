//=============================================================================
//
// �Q�[�������� [gamemain.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================

#include "gamemain.h"
#include "sound.h"
#include "scene.h"
#include "player.h"
#include "input.h"
#include "light.h"
#include "meshfield.h"
#include "shadow.h"
#include "camera.h"
#include "ball.h"
#include "bullet.h"
#include "wall.h"
#include "explo.h"
#include "score.h"
#include "effect.h"
#include "meshroll.h"
#include "meshdoom.h"
#include "enemy.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
PosCircle GetPlayer(void);
void SetPlayerFlag(int nLife);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
//���������� 
//*****************************************************************************
HRESULT InitGameMain(void)
{
	PlaySound(SOUND_LABEL_BGM001);
	//���C�g������
	if(FAILED(InitLight()))
	{
		return E_FAIL;
	}

	// ���f���̏���������
	if(FAILED(InitPlayer()))
	{
		return E_FAIL;
	}
	// �G�̏���������
	if(FAILED(InitEnemy()))
	{
		return E_FAIL;
	}
	//���b�V���t�B�[���h������
	if(FAILED(InitMeshField(10,10,FIELDSIZEMAX,FIELDSIZEMAX)))
	{
		return E_FAIL;
	}
	//�e������
	if(FAILED(InitShadow()))
	{
		return E_FAIL;
	}
	//�{�[��������
	if(FAILED(InitBall()))
	{
		return E_FAIL;
	}
	//���b�V���E�H�[��������
	if(FAILED(InitWall()))
	{
		return E_FAIL;
	}
	//�e������
	if(FAILED(InitBullet()))
	{
		return E_FAIL;
	}
	//����������
	if(FAILED(InitExplo()))
	{
		return E_FAIL;
	}
	//�G�t�F�N�g������
	if(FAILED(InitEffect()))
	{
		return E_FAIL;
	}
	//�X�R�A������
	if(FAILED(InitScore()))
	{
		return E_FAIL;
	}
	//�~��������
	if(FAILED(InitMeshRoll(12,1,150,50)))
	{
		return E_FAIL;
	}
	//�h�[��������
	if(FAILED(InitMeshDoom(12,12,FIELDSIZEMAX*100,FIELDSIZEMAX*100)))
	{
		return E_FAIL;
	}
		//�J�������C�g������
	if(FAILED(InitCamera()))
	{
		return E_FAIL;
	}
	return S_OK;

}
//*****************************************************************************
//�I������ 
//*****************************************************************************
void UninitGameMain(void)
{
	StopSound();
	//�J�������
	UninitCamera();
	//���C�g���
	UninitLight();
	// �e�̉��
	UninitShadow();
	//���f���̉��
	UninitPlayer();
	//���b�V���t�B�[���h���
	UninitMeshField();
	//�{�[�����
	UninitBall();
	//���b�V���E�H�[�����
	UninitWall();
	//�e���
	UninitBullet();
	//�������
	UninitExplo();
	//�G�t�F�N�g���
	UninitEffect();
	//�X�R�A���
	UninitScore();
	//�~�����
	UninitMeshRoll();
	//�h�[�����
	UninitMeshDoom();
	//�G���
	UninitEnemy();
}
//*****************************************************************************
//�X�V���� 
//*****************************************************************************
void UpdateGameMain(void)
{
	if(GetPosePlayer() != 1)
	{
		//�J�����X�V
		UpdateCamera();
		//���C�g�X�V
		UpdateLight();
		//�~���X�V
		UpdateMeshRoll();
		//�h�[���X�V
		UpdateMeshDoom();
		// ���f���̍X�V����
		UpdatePlayer();
		//���b�V���t�B�[���h�X�V
		UpdateMeshField();
		//�e�X�V
		UpdateShadow();
		//�{�[���X�V
		UpdateBall();
		//���b�V���E�H�[���X�V
		UpdateWall();
		//�e�X�V
		UpdateBullet();
		//�����X�V
		UpdateExplo();
		//�G�t�F�N�g�X�V
		UpdateEffect();
		//�X�R�A�X�V
		UpdateScore();
		//�G�X�V
		UpdateEnemy();
	}else{
#if _DEBUG
	PoseCameUpdate();
	if(GetKeyboardTrigger(DIK_P))
	{
		SetPosePlayer(false);
	}

#else
		PrintMessageProc("P�ōĊJ\nEnter�Ń^�C�g����\n");
		PrintMessageProc("WASD�ňړ�\nQE�Ŏ��_��]\nSPACE�������ςȂ��ŋ߂��̓G�Ɉ�����\n�����ƕ��d\n�G��5�񓖂���ƏI��\n�G�ɂ��߂����ŕ��d����Ƒ����X�R�A������I\n");
		if(GetKeyboardTrigger(DIK_P))
		{
			SetPosePlayer(false);
		}else
		if(GetKeyboardTrigger(DIK_RETURN))
		{
			SetNextScene(MODE_TITLE);
		}
#endif
	}
}
//*****************************************************************************
//�`�揈�� 
//*****************************************************************************
void DrawGameMain(void)
{
		//�J�����Z�b�g
		SetCamera();
		//�~���`��
		DrawMeshRoll();
		//�h�[���`��
		DrawMeshDoom();


				//���b�V���t�B�[���h�`��
		DrawMeshField();
		//���b�V���E�H�[���`��
		DrawWall();

				//�e�`��
		DrawShadow();
		// ���f���̕`�揈��
		DrawPlayer();
		//�e�`��
		DrawBullet();


		//�X�R�A�`��
		DrawScore();
		//�G�`��
		DrawEnemy();
				//�G�t�F�N�g�`��
		DrawEffect();
				//�����`��
		DrawExplo();
}

//End Of File