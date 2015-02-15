//=============================================================================
//
// �^�C�g���w�i [titlebg.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "titlelogo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_TITLELOGO ("data/TEXTURE/titlelogo.png")
#define LOGO_ROT_SPEED (0.05f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexTitleLogo(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int g_nTitleCnt;
int g_nLogoButtonCnt;
D3DXVECTOR3 g_fLogoSpeed;
D3DXVECTOR3 g_fLogoPos;
float g_fLogoRot;
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_fLogoRot = 0;
	g_fLogoSpeed = D3DXVECTOR3(0,0,0);
	g_fLogoPos = D3DXVECTOR3(TITLELOGO_X,TITLELOGO_Y,0);
	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffTitleLogo,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexTitleLogo();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_TITLELOGO,
								&g_pD3DTextureTitleLogo);
		g_nTitleCnt = 0;
	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitTitleLogo(void)
{
	if(g_pD3DVtxBuffTitleLogo!=NULL)
	{
		g_pD3DVtxBuffTitleLogo->Release();//������
		g_pD3DVtxBuffTitleLogo=NULL;
	}
		if(g_pD3DTextureTitleLogo!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateTitleLogo(void)
{
	g_nTitleCnt++;
	VERTEX_2D *pvtx;
	g_nTitleCnt = g_nTitleCnt%90;
	g_pD3DVtxBuffTitleLogo->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	for (int nLoop = 0; nLoop < 4 ; nLoop++)
	{

		if(g_nTitleCnt >=60)
		{
		if(g_nTitleCnt%3 == 0)
		{
		pvtx[nLoop].diffuse = D3DCOLOR_RGBA(20,20,200,255);//�F�ݒ�
		}else{
		pvtx[nLoop].diffuse = D3DCOLOR_RGBA(255,255,255,255);//�F�ݒ�
		}

		}
	}
	g_nLogoButtonCnt++;
	g_fLogoRot += LOGO_ROT_SPEED;
	if(g_fLogoRot<D3DX_PI)
	{
		g_fLogoRot = D3DX_PI;
	}
	g_fLogoPos.x -= sinf(g_fLogoRot);
	g_fLogoPos.y -= cosf(g_fLogoRot);
	//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y + TITLELOGO_H,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y+ TITLELOGO_H,0.0f);//���_�̍��W�i�[
	g_pD3DVtxBuffTitleLogo->Unlock();//���b�N�̉�����Y�ꂸ��

	g_pD3DVtxBuffTitleLogo->Unlock();//���b�N�̉�����Y�ꂸ��
}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|���S���̕`��

	pDevice->SetStreamSource(0,g_pD3DVtxBuffTitleLogo,0,sizeof(VERTEX_2D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_2D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureTitleLogo);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexTitleLogo(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffTitleLogo->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	
	//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y + TITLELOGO_H,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y+ TITLELOGO_H,0.0f);//���_�̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�

	pvtx[0].rhw = 1.0f;
	pvtx[1].rhw = 1.0f;
	pvtx[2].rhw = 1.0f;
	pvtx[3].rhw = 1.0f;

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,		0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1,1.0f);

g_pD3DVtxBuffTitleLogo->Unlock();//���b�N�̉�����Y�ꂸ��

}

//End Of File