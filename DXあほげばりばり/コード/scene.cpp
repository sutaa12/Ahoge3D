//=============================================================================
//
// �V�[���J�� [scene.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "scene.h"
#include "feed.h"
#include "gamemain.h"
#include "titlemain.h"
#include "resultmain.h"
#include "rankingmain.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FEED_SPEED (0.05f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitNextSecen(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//���������郂�[�h
MODE nInitMode;
//�I�����郂�[�h
MODE nUpdateMode;
//�`�悷�郂�[�h
MODE nDrawMode;
//�I�����郂�[�h
MODE nUninitMode;

//�t�F�[�h�l
float g_fFeed = 1.0f;
//*****************************************************************************
// �V�[���J�ڗp�֐��|�C���^
//*****************************************************************************
HRESULT (*ModeInit[MODE_MAX])(void)
=
{
	InitTitle,
	InitGameMain,
	InitResult,
	RankingInit,
	NULL,
	NULL
};

void (*ModeUpdate[MODE_MAX])(void)
=
{
	UpdateTitle,
	UpdateGameMain,
	UpdateResult,
	RankingUpdate,
	NULL,
	NULL
};

void (*ModeDraw[MODE_MAX])(void)
=
{
	DrawTitle,
	DrawGameMain,
	DrawResult,
	RankingDraw,
	NULL,
	NULL
};

void (*ModeUninit[MODE_MAX])(void)
=
{
	UninitTitle,
	UninitGameMain,
	UninitResult,
	RankingUninit,
	NULL,
	NULL
};

//*****************************************************************************
// �V�[��������
//*****************************************************************************
void InitScene(void)
{
	//�Q�[�����[�h������
	nUpdateMode = MODE_FADE;
	//���ɏ��������郂�[�h
	nInitMode = MODE_TITLE;
	nUninitMode = MODE_NONE;
	nDrawMode = MODE_NONE;
	//�t�F�[�h�ݒ�
	g_fFeed = 1;
	//�t�F�[�h������
	InitFeed();
}
//*****************************************************************************
// �V�[�����
//*****************************************************************************
void UninitScene(void)
{
	//�A�b�v�f�[�g���k�łȂ��t�F�[�h�ł��Ȃ��Ȃ�
	if(nUpdateMode != MODE_NONE && nUpdateMode != MODE_FADE)
	{
	//�V�[�����
	(ModeUninit[nUpdateMode])();
	}
	//�t�F�[�h���
	UninitFeed();
}
//*****************************************************************************
// �V�[���X�V
//*****************************************************************************
void UpdateScene(void)
{
	//�������ɓ����ĂȂ��A�b�v�f�[�g�ɓ����Ă��ăt�F�[�h�łȂ��I�����Ȃ��ꍇ
	if(nInitMode == MODE_NONE && (nUpdateMode != MODE_NONE) && (nUpdateMode != MODE_FADE) &&	 nUninitMode == MODE_NONE)
	{
		(*ModeUpdate[nUpdateMode])();//�A�b�v�f�[�g�֐��Ăяo��
	}
	//�t�F�[�h����
	if(nUpdateMode == MODE_FADE)
	{
		g_fFeed += FEED_SPEED;
		//�t�F�[�h��1�𒴂�����
		if(g_fFeed > 1)
		{
			//�I����������łȂ��ꍇ
			if(ModeUninit[nUninitMode] !=NULL)
			{
				(*ModeUninit[nUninitMode])();
			}
			//��������������łȂ��ꍇ
			if(ModeInit[nInitMode] != NULL)
			{
				(*ModeInit[nInitMode])();
				//�X�V�̃V�[���ԍ����������̂ɓ����
				nUpdateMode = nInitMode;
				//�`����Z�b�g
				nDrawMode = nInitMode;
				//�������ƏI�������
				nInitMode = MODE_NONE;
				nUninitMode = MODE_NONE;
			}
		}
		
	}else{
			//�t�F�[�h�A�E�g
			if(g_fFeed > 0)
			{
				g_fFeed -= FEED_SPEED;
			}
			else
			//�t�F�[�h��0�ȉ��Ȃ�O�ɂ���
			if(g_fFeed < 0)
			{
				g_fFeed = 0;
			}
	}
	//�t�F�[�h��1�ȏ�Ȃ�1�ɂ���
	if(g_fFeed > 1)
	{
		g_fFeed = 1;
	}
	//�t�F�[�h�X�V
	UpdateFeed(g_fFeed);
}
//*****************************************************************************
// �V�[���`��
//*****************************************************************************
void DrawScene(void)
{
	if(nDrawMode != MODE_NONE)
	{
	//�`��
	(ModeDraw[nDrawMode])();
	}
	//�t�F�[�h�`��
	DrawFeed();
}
//*****************************************************************************
// ���̃V�[������	
//*****************************************************************************
void SetNextScene(MODE nMode)
{
	//��̃��[�h�łȂ����
	if(nMode !=	MODE_NONE)
	{
		//����ɃA�b�v�f�[�g������
		nUninitMode =	 nUpdateMode;
		//�t�F�[�h�ɕύX
		nUpdateMode = MODE_FADE;
		//�������Z�b�g
		nInitMode = nMode;
	}
}
//*****************************************************************************
// �V�[��������
//*****************************************************************************
void InitNextScene(void)
{
	//����������
	(*ModeInit[nInitMode])();
}
	
//End Of File