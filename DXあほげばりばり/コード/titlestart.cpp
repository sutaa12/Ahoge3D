//=============================================================================
//
// �^�C�g���{�^�� [titlestart.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "titlestart.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_TITLELSTART ("data/TEXTURE/start.png")
#define START_ROT_SPEED (0.1f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexTitleStart(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleStart = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleStart = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int g_nStartButtonCnt;
D3DXVECTOR3 g_fStartSpeed;
D3DXVECTOR3 g_fStartPos;
float g_fStartRot;
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitTitleStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_fStartRot = 0;
	g_fStartSpeed = D3DXVECTOR3(0,0,0);
	g_fStartPos = D3DXVECTOR3(TITLESTART_X,TITLESTART_Y,0);
	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffTitleStart,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexTitleStart();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_TITLELSTART,
								&g_pD3DTextureTitleStart);
		g_nStartButtonCnt = 0;
	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitTitleStart(void)
{
	if(g_pD3DVtxBuffTitleStart!=NULL)
	{
		g_pD3DVtxBuffTitleStart->Release();//������
		g_pD3DVtxBuffTitleStart=NULL;
	}
		if(g_pD3DTextureTitleStart!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureTitleStart->Release();
		g_pD3DTextureTitleStart=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateTitleStart(void)
{
	g_nStartButtonCnt++;
	g_fStartRot += START_ROT_SPEED;
	if(g_fStartRot>D3DX_PI)
	{
		g_fStartRot = -D3DX_PI;
	}
	g_fStartPos.x += sinf(g_fStartRot);
	g_fStartPos.y += cosf(g_fStartRot);
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffTitleStart->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	
	//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y + TITLESTART_H,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y+ TITLESTART_H,0.0f);//���_�̍��W�i�[
	g_pD3DVtxBuffTitleStart->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawTitleStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|���S���̕`��

	pDevice->SetStreamSource(0,g_pD3DVtxBuffTitleStart,0,sizeof(VERTEX_2D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_2D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureTitleStart);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexTitleStart(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffTitleStart->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	
	//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y + TITLESTART_H,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y+ TITLESTART_H,0.0f);//���_�̍��W�i�[
	for(int loop=0;loop<4;loop++)
	{
	pvtx[loop].diffuse = D3DCOLOR_RGBA(255,255,20,255); //�F�ݒ�
	
	pvtx[loop].rhw = 1.0f;
	}
	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,		0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1,1.0f);

g_pD3DVtxBuffTitleStart->Unlock();//���b�N�̉�����Y�ꂸ��

}

//End Of File