//=============================================================================
//
// �n�ʏ��� [wall.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "wall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SIZE_X (400)
#define SIZE_Z (500)
#define TEX_FIELD ("data/TEXTURE/wall.dds")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureWall = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxviewW;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojectionW;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworldW;		//���[���h�}�g���b�N�X


D3DXVECTOR3 g_posWall;		//�n�ʂ̈ʒu
D3DXVECTOR3 g_rotWall;		//�n�ʂ̌���
D3DXVECTOR3 g_sclWall;		//�n�ʂ̑傫��
//=============================================================================
// ����������
//=============================================================================
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�B�[���h������
	g_posWall = D3DXVECTOR3(0.0,0.0,0.0);
	g_rotWall = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclWall = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffWall,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexWall();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureWall);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWall(void)
{
		if(g_pD3DVtxBuffWall!=NULL)
	{
		g_pD3DVtxBuffWall->Release();//������
		g_pD3DVtxBuffWall=NULL;
	}
		if(g_pD3DTextureWall!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureWall->Release();
		g_pD3DTextureWall=NULL;//�A�h���X�����
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWall(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	float rot_y=0;

	D3DXMatrixIdentity(&g_mtxworldW);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,0.0f,0.0f,FIELDSIZEMAX);//XYZ
	D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldW);

	//�|���S���̕`��
	pDevice->SetStreamSource(0,g_pD3DVtxBuffWall,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureWall);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffWall->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	for(int nLoop=0;nLoop<3;nLoop++)
	{
			g_pD3DVtxBuffWall->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

		rot_y+=D3DX_PI/2;
		D3DXMatrixRotationYawPitchRoll(&mtxRot,rot_y,0.0f,0.0f);//YXZ
		D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxRot);//��]�̔��f
						for(int nLoop2=0;nLoop2<4;nLoop2++)
			{
				pvtx[nLoop2].nor = D3DXVECTOR3(0.0f,0,-1.0f);//�@���̍��W�i�[
			}
		if(nLoop==2)
		{
		D3DXMatrixTranslation(&mtxTranslate,FIELDSIZEMAX,0,0);//XYZ
		}else
		if(nLoop==1)
		{
		D3DXMatrixTranslation(&mtxTranslate,FIELDSIZEMAX,0,FIELDSIZEMAX);//XYZ
		}else
		if(nLoop==0)
		{
				for(int nLoop2=0;nLoop2<4;nLoop2++)
			{
				pvtx[nLoop2].nor = D3DXVECTOR3(0.0f,0,-1.0f);//�@���̍��W�i�[
			}
			D3DXMatrixTranslation(&mtxTranslate,0,0,FIELDSIZEMAX);//XYZ
		}
		D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxTranslate);//���[���h�̔��f
		pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldW);



	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��
	}


	g_pD3DVtxBuffWall->Unlock();//����

}

//=============================================================================
// ���_�̏���������
//=============================================================================
void InitVertexWall(void)
{

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffWall->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,SIZE_Z,0);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(FIELDSIZEMAX,SIZE_Z,0);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(FIELDSIZEMAX,0.0f,0);//���_�̍��W�i�[

	pvtx[0].nor = D3DXVECTOR3(0.0f,0,1.0f);//�@���̍��W�i�[
	pvtx[1].nor = D3DXVECTOR3(0.0f,0,1.0f);//�@���̍��W�i�[
	pvtx[2].nor = D3DXVECTOR3(0.0f,0,1.0f);//�@���̍��W�i�[
	pvtx[3].nor = D3DXVECTOR3(0.0f,0,1.0f);//�@���̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pD3DVtxBuffWall->Unlock();//���b�N�̉�����Y�ꂸ��

}

//End Of File