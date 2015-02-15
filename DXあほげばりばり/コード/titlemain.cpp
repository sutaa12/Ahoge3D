//=============================================================================
//
// �^�C�g������ [titlemain.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include"light.h"
#include "titlemain.h"
#include "titlelogo.h"
#include "scene.h"
#include "titlecamera.h"
#include "titleplayer.h"
#include "titleenemy.h"
#include "input.h"
#include "sound.h"
#include "titlestart.h"
#include "meshdoom.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int g_TitleCOUNT;
//*****************************************************************************
//���������� 
//*****************************************************************************
HRESULT  InitTitle(void)
{
	InitLight();
	InitTitleCamera();
	InitTitlePlayer();
	InitMeshDoom(20,20,10000,10000);
	InitTitleStart();
	//�w�i������
	if(FAILED(InitTitleLogo()))
	{
		return E_FAIL;
	}
	PlaySound(SOUND_LABEL_BGM000);
	g_TitleCOUNT = 0;
	return S_OK;
}
//*****************************************************************************
//�I������ 
//*****************************************************************************
void UninitTitle(void)
{
	UninitLight();
	UninitTitlePlayer();
	//�w�i���
	UninitTitleLogo();
	UninitTitleStart();
	UninitMeshDoom();
	StopSound();
}
//*****************************************************************************
//�X�V���� 
//*****************************************************************************
void UpdateTitle(void)
{
	UpdateLight();
	UpdateTitlePlayer();
	UpdateTitleCamera();
	UpdateTitleStart();
	UpdateMeshDoom();
	//�w�i�X�V
	UpdateTitleLogo();
	//PrintMessageProc("���ٖуo���o����DX�ł̂悤�Ȃ���\n");

	PrintMessageProc("\n\n\n\n\n\n\n\n\nWASD�ňړ�\nQE�Ŏ��_��]\nSPACE�������ςȂ���\n�߂��̓G�Ɉ�����\n�����ƕ��d\n�G��5�񓖂���ƏI��\n�G�ɂ��߂����ŕ��d����Ƒ����X�R�A������I\n");
	//PrintMessageProc("ENTER�ŃX�^�[�g\n");
	//�{�^���ŃV�[���؂�ւ�
	if(GetKeyboardTrigger(DIK_RETURN))
	{
		SetNextScene(MODE_GAMEMAIN);
	}
	if(g_TitleCOUNT  > 30 * 60)
	{
		SetNextScene(MODE_RANKING);
	}
	g_TitleCOUNT++;
}
//*****************************************************************************
//�`�揈�� 
//*****************************************************************************
void DrawTitle(void)
{
	DrawMeshDoom();
	SetTitleCamera();
	DrawTitlePlayer();

	DrawTitleStart();	
		//�w�i�`��
	DrawTitleLogo();
}
