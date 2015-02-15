//=============================================================================
//
// �t�F�[�h���� [feed.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "feed.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_BULLET (" ")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexFeed(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureFeed = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFeed = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitFeed(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffFeed,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	//���_������
	InitVertexFeed();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureFeed);
	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitFeed(void)
{
	if(g_pD3DVtxBuffFeed!=NULL)
	{
		g_pD3DVtxBuffFeed->Release();//������
		g_pD3DVtxBuffFeed=NULL;
	}
		if(g_pD3DTextureFeed!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureFeed->Release();
		g_pD3DTextureFeed=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateFeed(float fFeed)
{
		VERTEX_2D *pvtx;
	g_pD3DVtxBuffFeed->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //�F�ݒ�

	g_pD3DVtxBuffFeed->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawFeed(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|���S���̕`��

	pDevice->SetStreamSource(0,g_pD3DVtxBuffFeed,0,sizeof(VERTEX_2D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_2D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureFeed);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexFeed(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffFeed->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	
	//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,0,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);//���_�̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�

	pvtx[0].rhw = 1.0f;
	pvtx[1].rhw = 1.0f;
	pvtx[2].rhw = 1.0f;
	pvtx[3].rhw = 1.0f;

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f/10,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f/10,1.0f);

g_pD3DVtxBuffFeed->Unlock();//���b�N�̉�����Y�ꂸ��

}

//End Of File