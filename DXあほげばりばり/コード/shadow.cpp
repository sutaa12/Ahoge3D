//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "shadow.h"
#include "camera.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SIZE_X (20)
#define SIZE_Z (20)
#define TEX_FIELD ("data/TEXTURE/shadow000.jpg")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void InitVertexShadow(void);
D3DXVECTOR3 GetEnemyPosition(int nNum);
SHADOWDATA g_posshadow[SHADOW_MAX];

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureShadow = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffShadow = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxviewF;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojectionF;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworldF;		//���[���h�}�g���b�N�X


D3DXVECTOR3 g_posShadow;		//�n�ʂ̈ʒu
D3DXVECTOR3 g_rotShadow;		//�n�ʂ̌���
D3DXVECTOR3 g_sclShadow;		//�n�ʂ̑傫��
//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�B�[���h������
	g_posShadow = D3DXVECTOR3(0.0,0.0,0.0);
	g_rotShadow = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclShadow = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffShadow,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexShadow();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureShadow);

	for(int nLoop = 0;nLoop < SHADOW_MAX;nLoop++)
	{
		g_posshadow[nLoop].nPosx = 0;
		g_posshadow[nLoop].nPosz = 0;

	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
		if(g_pD3DVtxBuffShadow!=NULL)
	{
		g_pD3DVtxBuffShadow->Release();//������
		g_pD3DVtxBuffShadow=NULL;
	}
		if(g_pD3DTextureShadow!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureShadow->Release();
		g_pD3DTextureShadow=NULL;//�A�h���X�����
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{
	for(int nLoop = 0;nLoop < SHADOW_MAX;nLoop++)
	{
		if(GetBulletFlag(nLoop) == 1)
		{
			D3DXVECTOR3 vector = GetBulletPos(nLoop);
			g_posshadow[nLoop].nPosx = vector.x;
			g_posshadow[nLoop].nPosz = vector.z;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;
	
	//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//�v���C���[�̉e�`��
	D3DXMatrixIdentity(&g_mtxworldF);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,GetPlayerPosition().x - 8,0.01f,GetPlayerPosition().z - 8);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldF);


	//�|���S���̕`��


	pDevice->SetStreamSource(0,g_pD3DVtxBuffShadow,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureShadow);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��

	//�G�̉e�`��
	for(int nLoop = 0;nLoop < ENEMY_MAX;nLoop++)
	{
		if(GetEnemyLife(nLoop) == 1)
		{
			D3DXMatrixIdentity(&g_mtxworldF);
			D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
			D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxScl);//�X�P�[���̔��f

			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
			D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxRot);//��]�̔��f

			D3DXMatrixTranslation(&mtxTranslate,GetEnemyPosition(nLoop).x,0.01f,GetEnemyPosition(nLoop).z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxTranslate);//���[���h�̔��f

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldF);


			//�|���S���̕`��


			pDevice->SetStreamSource(0,g_pD3DVtxBuffShadow,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

			pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

			pDevice->SetTexture(0,g_pD3DTextureShadow);//�e�N�X�`���Z�b�g

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
											0,
											2);//��
		}

	}

	for(int nLoop = 0;nLoop < SHADOW_MAX;nLoop++)
	{
		if(GetBulletFlag(nLoop) == 1)
		{
			D3DXVECTOR3 vector = GetBulletPos(nLoop);
				D3DXMatrixIdentity(&g_mtxworldF);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxRot);//��]�̔��f
	D3DXMatrixTranslation(&mtxTranslate,g_posshadow[nLoop].nPosx,0.01f,g_posshadow[nLoop].nPosz);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldF);


	//�|���S���̕`��

	//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);

	pDevice->SetStreamSource(0,g_pD3DVtxBuffShadow,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureShadow);//�e�N�X�`���Z�b�g

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									2);//��
		}
	}

		//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);


}

//=============================================================================
// ���_�̏���������
//=============================================================================
void InitVertexShadow(void)
{

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffShadow->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,0.0f,SIZE_Z);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(SIZE_X,0.0f,SIZE_Z);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(SIZE_X,0.0f,0);//���_�̍��W�i�[

	pvtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//�@���̍��W�i�[
	pvtx[1].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//�@���̍��W�i�[
	pvtx[2].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//�@���̍��W�i�[
	pvtx[3].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//�@���̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,150); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,150); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,150); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,150); //�F�ݒ�

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pD3DVtxBuffShadow->Unlock();//���b�N�̉�����Y�ꂸ��

}

//End Of File