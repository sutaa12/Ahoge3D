//=============================================================================
//
// ���ʔw�i [resultbg.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "resultbg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_BULLET (" ")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexResultBG(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultBG = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultBG = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
int g_nResultFlag = 0;
int g_nResultScore;//���U���g�p�̃X�R�A�̒l�ێ�
int g_nNumEnemyScore;//�G�����̓|������
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitResultBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffResultBG,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexResultBG();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureResultBG);

		g_nResultScore += g_nNumEnemyScore * 25;
	FILE *fp;
	fp=fopen(FILE_R,"ab");
	fwrite(&g_nResultScore,sizeof(int),1,fp);
	fclose(fp);

	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitResultBG(void)
{
	if(g_pD3DVtxBuffResultBG!=NULL)
	{
		g_pD3DVtxBuffResultBG->Release();//������
		g_pD3DVtxBuffResultBG=NULL;
	}
		if(g_pD3DTextureResultBG!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureResultBG->Release();
		g_pD3DTextureResultBG=NULL;//�A�h���X�����
	}

}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateResultBG(void)
{
	if(g_nResultFlag == 0)
	{
	//���ʔw�i�X�V
	PrintMessageProc("\n\n\n\n\n\n\n\n���ʉ��\n");
	PrintMessageProc("�|�����G�̐�: %d\n",g_nNumEnemyScore);
	PrintMessageProc("�G�̐��̃X�R�A: %d\n",g_nNumEnemyScore * 25);
	PrintMessageProc("�X�R�A: %d\n",g_nResultScore);
	PrintMessageProc("�G���^�[�Ń^�C�g����");
	}else{
	//���ʔw�i�X�V
	PrintMessageProc("�Q�[���I�[�o�[\n");
	PrintMessageProc("�G���^�[�Ń^�C�g����");
	}
}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawResultBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|���S���̕`��

	pDevice->SetStreamSource(0,g_pD3DVtxBuffResultBG,0,sizeof(VERTEX_2D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_2D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureResultBG);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									4);//��
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexResultBG(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffResultBG->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	
	//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,0,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);//���_�̍��W�i�[
	if(g_nResultFlag == 0)
	{
	pvtx[0].diffuse = D3DCOLOR_RGBA(55,55,255,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(55,55,255,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(55,55,255,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(55,55,255,255); //�F�ݒ�
	}else{
	pvtx[0].diffuse = D3DCOLOR_RGBA(255,25,25,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,25,25,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,25,25,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,25,25,255); //�F�ݒ�
	}
	pvtx[0].rhw = 1.0f;
	pvtx[1].rhw = 1.0f;
	pvtx[2].rhw = 1.0f;
	pvtx[3].rhw = 1.0f;

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f/10,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f/10,1.0f);

g_pD3DVtxBuffResultBG->Unlock();//���b�N�̉�����Y�ꂸ��

}

void SetResultMode(int nMode)
{
	g_nResultFlag = nMode;
}

//*****************************************************************************
// �X�R�A�̒l�킽��
//*****************************************************************************
void SetScoreNum(int nNum)
{
	g_nResultScore = nNum;
}
//*****************************************************************************
// �G�̐��킽��
//*****************************************************************************
void SetEnemyNum(int nNum)
{
	g_nNumEnemyScore = nNum;
}

//End Of File