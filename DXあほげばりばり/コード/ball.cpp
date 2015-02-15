//=============================================================================
//
// �r���{�[�h���� [ball.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "ball.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SIZE_X (20)
#define SIZE_Z (20)
#define TEX_FIELD ("data/TEXTURE/bullet000.png")
#define MOVE_POS (5)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****��************************************************************************

void InitVertexBall(void);
D3DXMATRIX GetMatrixView(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBall = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBall = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxviewB;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojectionB;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworldB;		//���[���h�}�g���b�N�X


D3DXVECTOR3 g_posBall;		//�n�ʂ̈ʒu
D3DXVECTOR3 g_rotBall;		//�n�ʂ̌���
D3DXVECTOR3 g_sclBall;		//�n�ʂ̑傫��
//=============================================================================
// ����������
//=============================================================================
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�B�[���h������
	g_posBall = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotBall = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclBall = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffBall,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexBall();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureBall);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBall(void)
{
		if(g_pD3DVtxBuffBall!=NULL)
	{
		g_pD3DVtxBuffBall->Release();//������
		g_pD3DVtxBuffBall=NULL;
	}
		if(g_pD3DTextureBall!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureBall->Release();
		g_pD3DTextureBall=NULL;//�A�h���X�����
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBall(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;
	mtxView = GetMatrixView();//�J�����̃r���[���Ƃ��Ă���
	D3DXMatrixIdentity(&g_mtxworldB);//�{�[����Identey������
	D3DXMatrixInverse(&g_mtxworldB,NULL,&mtxView);//�t�s��
	g_mtxworldB._41 = 0.0f;
	g_mtxworldB._42 = 0.0f;
	g_mtxworldB._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldB,&g_mtxworldB,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldB,&g_mtxworldB,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,g_posBall.x,g_posBall.y,g_posBall.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldB,&g_mtxworldB,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldB);




	//�|���S���̕`��

	pDevice->SetStreamSource(0,g_pD3DVtxBuffBall,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureBall);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��


}

//=============================================================================
// ���_�̏���������
//=============================================================================
void InitVertexBall(void)
{

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffBall->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,SIZE_Z,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(SIZE_X,SIZE_Z,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(SIZE_X,0.0f,0);//���_�̍��W�i�[

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

g_pD3DVtxBuffBall->Unlock();//���b�N�̉�����Y�ꂸ��

}

D3DXVECTOR3 GetEnemyField()
{
	return g_posBall;
}

//End Of FIle