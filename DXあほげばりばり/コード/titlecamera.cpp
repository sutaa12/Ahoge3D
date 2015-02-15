//=============================================================================
//
// �J�������� [Titlecamera.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "titlecamera.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CT_MOVEX (1)
#define CT_MOVEY (1)
#define CT_MOVEZ (1)
#define CT_ROTMOVE (0.01f)
#define CT_POSY (60)
#define CT_MODELPOS (160)
#define CT_SPEED (0.15f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3 g_posTitlecameraP;	//�J�����̈ʒu
D3DXVECTOR3 g_posTitlecameraR;	//�J�����̒����_
D3DXVECTOR3 g_vecTitlecameraV;	//�J�����̕����x�N�g��

D3DXVECTOR3 g_posTitlecameraPDest;	//�ړI�̃J�����̈ʒu
D3DXVECTOR3 g_posTitlecameraRDest;	//�ړI�̃J�����̒����_

D3DXMATRIX g_tcmtxview;		//�r���[�}�g���b�N�X
D3DXMATRIX g_tcmtxprojection;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_tcmtxworld;		//���[���h�}�g���b�N�X


float g_vecTitlecamera;	//�J���������_�܂ł̋���
D3DXVECTOR3 g_rotTitlecamera;	//�J�����̌���


//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitTitleCamera(void)
{
	g_rotTitlecamera = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_posTitlecameraP = D3DXVECTOR3(0.0f,0,0);
	g_posTitlecameraR = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,-5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);
	g_vecTitlecameraV = D3DXVECTOR3(0.0f,1.0f,0.0f);

	g_vecTitlecamera = 100;
	//�J�����̈ʒu�̍����ݒ�	
	g_posTitlecameraPDest = D3DXVECTOR3(0.0f,CT_POSY,50.0f);
	//�J�����̉�]�̍����ݒ�
	g_posTitlecameraRDest = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);
	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitTitleCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateTitleCamera(void)
{
	g_rotTitlecamera = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_posTitlecameraP = D3DXVECTOR3(0.0f,0,0);
	g_posTitlecameraR = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,-5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);
	g_vecTitlecameraV = D3DXVECTOR3(0.0f,1.0f,0.0f);

	g_vecTitlecamera = 100;
	//�J�����̈ʒu�̍����ݒ�	
	g_posTitlecameraPDest = D3DXVECTOR3(0.0f,CT_POSY,50.0f);
	//�J�����̉�]�̍����ݒ�
	g_posTitlecameraRDest = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);

	#ifdef _DEBUG
	PrintDebugProc("�J�������W(X:%.2fY:%.2fZ:%.2f)\n",g_posTitlecameraP.x,g_posTitlecameraP.y,g_posTitlecameraP.z);
	PrintDebugProc("�J���������_(X:%.2fY:%.2fZ:%.2f)\n",g_posTitlecameraR.x,g_posTitlecameraR.y,g_posTitlecameraR.z);
	PrintDebugProc("�J��������(%.3f),���x(X:%.2f,Z:%.2f)\n",g_rotTitlecamera.y,g_posTitlecameraRDest.x,g_posTitlecameraRDest.z);
	#endif

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetTitleCamera(void)
{
	D3DXMATRIX tcmtxScl,tcmtxRot,tcmtxTranslate;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//�r���[�}�g���b�N�X
	D3DXMatrixIdentity(&g_tcmtxview);
	D3DXMatrixLookAtLH(&g_tcmtxview,&g_posTitlecameraP,&g_posTitlecameraR,&g_vecTitlecameraV);
	pDevice->SetTransform(D3DTS_VIEW,&g_tcmtxview);
	D3DXMatrixIdentity(&g_tcmtxprojection);
	D3DXMatrixPerspectiveFovLH(	&g_tcmtxprojection,//�}�g���b�N�X�쐬
								(D3DX_PI/4),//����
								(float)SCREEN_WIDTH/SCREEN_HEIGHT,//�A�X�y�N�g��
								10.0f,	//����
								10000.0f);//�`�拗��
	pDevice->SetTransform(D3DTS_PROJECTION,&g_tcmtxprojection);

		//�u���W�F�N�g�}�g���b�N�X
	D3DXMatrixIdentity(&g_tcmtxworld);
	D3DXMatrixScaling(&tcmtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_tcmtxworld,&g_tcmtxworld,&tcmtxScl);//�X�P�[���̔��f

}

//End Of File