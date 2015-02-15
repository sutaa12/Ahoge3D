//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9 g_aLight[3];		//���C�g���

//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	D3DXVECTOR3 vecDir;

	ZeroMemory(&g_aLight[0],sizeof(D3DLIGHT9)*3);//���C�g������

	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;//���s�����@���
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	vecDir = D3DXVECTOR3(1.0f,-1.0f,-1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction,&vecDir);
	pDevice->SetLight(0,&g_aLight[0]);
	pDevice->LightEnable(0,TRUE);
	//0

	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;//���s�����@���
	g_aLight[1].Diffuse = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
	vecDir = D3DXVECTOR3(0.1f,-1.0f,0.9f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction,&vecDir);
	pDevice->SetLight(1,&g_aLight[1]);
	pDevice->LightEnable(1,TRUE);
	//1

	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;//���s�����@���
	g_aLight[2].Diffuse = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	vecDir = D3DXVECTOR3(-0.9f,0.3f,-0.5f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction,&vecDir);
	pDevice->SetLight(2,&g_aLight[2]);
	pDevice->LightEnable(2,TRUE);
	//2
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

	return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{
}

//End Of File