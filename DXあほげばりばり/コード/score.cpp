//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "score.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_BULLET ("data/TEXTURE/number000.png")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexScore(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffScore = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int g_nNumber = 0;
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_2D)*4*(UINT)SCORE_MAX,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffScore,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexScore();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureScore);
	g_nNumber = 0;

	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitScore(void)
{
	if(g_pD3DVtxBuffScore!=NULL)
	{
		g_pD3DVtxBuffScore->Release();//������
		g_pD3DVtxBuffScore=NULL;
	}
		if(g_pD3DTextureScore!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureScore->Release();
		g_pD3DTextureScore=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateScore(void)
{

	VERTEX_2D *pvtx;
	int noldNumber = 0;
	int nNumberC = g_nNumber;
	
	g_pD3DVtxBuffScore->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
//�X�R�A�̃Z�b�g	
for(int nCnt = 1;nCnt <= SCORE_MAX ; nCnt++)
	{
		noldNumber = (int)(nNumberC / pow(10,(double)(SCORE_MAX - nCnt)));
		pvtx[0].tex = D3DXVECTOR2((float)noldNumber / 10.0f,0.0f);
		pvtx[1].tex = D3DXVECTOR2((float)(noldNumber / 10.0f + 0.1),0.0f);
		pvtx[2].tex = D3DXVECTOR2((float)noldNumber / 10.0f,1.0f);
		pvtx[3].tex = D3DXVECTOR2((float)noldNumber / 10.0f+0.1f,1.0f);
		pvtx += 4;
		nNumberC -= (int)(noldNumber * pow(10,(double)(SCORE_MAX - nCnt)));
	}
	g_pD3DVtxBuffScore->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|���S���̕`��

	pDevice->SetStreamSource(0,g_pD3DVtxBuffScore,0,sizeof(VERTEX_2D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_2D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureScore);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									4*(UINT)SCORE_MAX);//��
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexScore(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffScore->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	for(int nCnt = 0;nCnt < SCORE_MAX ; nCnt++)
	{
				//���_���̏�����

	pvtx[0].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt),SCORE_Y,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt)+SCORE_W,SCORE_Y,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt),SCORE_Y+SCORE_H,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt)+SCORE_W,SCORE_Y+SCORE_H,0.0f);//���_�̍��W�i�[

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
	pvtx += 4;
	}
g_pD3DVtxBuffScore->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// ���Z
//*****************************************************************************
void AddScore(int nNum)
{
	g_nNumber += nNum;
	//�ő包����������
	if(g_nNumber>pow((float)10,(float)SCORE_MAX))
	{
		g_nNumber = (int)pow((float)10,(float)SCORE_MAX) - 1;
	}else
	if(g_nNumber < 0)
	{
		g_nNumber = 0;
	}


}
//*****************************************************************************
// ���ݒl�擾
//*****************************************************************************
int GetScore(void)
{
	return g_nNumber;
}
//End Of File