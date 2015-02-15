//=============================================================================
//
// �e�̏��� [explo.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "explo.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_BULLET ("data/TEXTURE/shadow000.jpg")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
D3DXMATRIX GetMatrixView(void);
void InitVertexExplo(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureExplo = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExplo = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxviewExplo;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojectionExplo;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworldExplo;		//���[���h�}�g���b�N�X


D3DXVECTOR3 g_posExplo;		//�n�ʂ̈ʒu
D3DXVECTOR3 g_rotExplo;		//�n�ʂ̌���
D3DXVECTOR3 g_sclExplo;		//�n�ʂ̑傫��

EXPLODATA g_ExploData[E_MAX];//�e�f�[�^�̔z��
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitExplo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�B�[���h������
	g_posExplo = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotExplo = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclExplo = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffExplo,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexExplo();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureExplo);
	//�z�񏉊���
		for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{

		//�t���O�ƍ��W�Z�b�g
		g_ExploData[nLoop].nFlag = 0;
		g_ExploData[nLoop].nPosx = 0;
		g_ExploData[nLoop].nPosy = 0;
		g_ExploData[nLoop].nPosz = 0;
		g_ExploData[nLoop].fSizex = 1.0f;
		g_ExploData[nLoop].fSizey = 1.0f;
		g_ExploData[nLoop].fSizez = 1.0f;
		g_ExploData[nLoop].fAlpha = 1.0f;
		g_ExploData[nLoop].nRot = 0;

	}
	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitExplo(void)
{
	if(g_pD3DVtxBuffExplo!=NULL)
	{
		g_pD3DVtxBuffExplo->Release();//������
		g_pD3DVtxBuffExplo=NULL;
	}
		if(g_pD3DTextureExplo!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureExplo->Release();
		g_pD3DTextureExplo=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateExplo(void)
{
	for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{
		if(g_ExploData[nLoop].nFlag == 1)
		{
			g_ExploData[nLoop].fSizex += E_SIZESPEED;//�T�C�Y�ύX
			g_ExploData[nLoop].fSizey += E_SIZESPEED;//�T�C�Y�ύX
			g_ExploData[nLoop].fSizez += E_SIZESPEED;//�T�C�Y�ύX
			g_ExploData[nLoop].fAlpha -= E_ALPHASPEED;//�T�C�Y�ύX
			//����0�ɂȂ�����0�ɂ���
			if(g_ExploData[nLoop].fAlpha < 0)
			{
				g_ExploData[nLoop].nFlag = 0;

			}
		}
	}
}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawExplo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;

	
		//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	mtxView = GetMatrixView();//�J�����̃r���[���Ƃ��Ă���

	D3DXMatrixIdentity(&g_mtxworldExplo);//Identey������
	D3DXMatrixInverse(&g_mtxworldExplo,NULL,&mtxView);//�t�s��
	g_mtxworldExplo._41 = 0.0f;
	g_mtxworldExplo._42 = 0.0f;
	g_mtxworldExplo._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,g_posExplo.x,g_posExplo.y,g_posExplo.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldExplo);

	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);
	for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{
		if(g_ExploData[nLoop].nFlag == 1)//�t���O�������Ă��Ȃ���
		{
				VERTEX_3D *pvtx;
				g_pD3DVtxBuffExplo->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				pvtx[0].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //�F�ݒ�
				pvtx[1].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //�F�ݒ�
				pvtx[2].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //�F�ݒ�
				pvtx[3].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //�F�ݒ�

				g_pD3DVtxBuffExplo->Unlock();//���b�N�̉�����Y�ꂸ��


			D3DXMatrixIdentity(&g_mtxworldExplo);//�{�[����Identey������
			D3DXMatrixInverse(&g_mtxworldExplo,NULL,&mtxView);//�t�s��
			g_mtxworldExplo._41 = 0.0f;
			g_mtxworldExplo._42 = 0.0f;
			g_mtxworldExplo._43 = 0.0f;

			g_posExplo = D3DXVECTOR3(g_ExploData[nLoop].fSizex,g_ExploData[nLoop].fSizey,g_ExploData[nLoop].fSizez);

			D3DXMatrixScaling(&mtxScl,g_posExplo.x,g_posExplo.y,g_posExplo.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxScl);//�X�P�[���̔��f

			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
			D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxRot);//��]�̔��f

			g_posExplo = D3DXVECTOR3(	g_ExploData[nLoop].nPosx,
									g_ExploData[nLoop].nPosy,
									g_ExploData[nLoop].nPosz);

			D3DXMatrixTranslation(&mtxTranslate,g_posExplo.x,g_posExplo.y,g_posExplo.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxTranslate);//���[���h�̔��f

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldExplo);

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldExplo);
			//�|���S���̕`��

			pDevice->SetStreamSource(0,g_pD3DVtxBuffExplo,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

			pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

			pDevice->SetTexture(0,g_pD3DTextureExplo);//�e�N�X�`���Z�b�g
			for(int nCnt = 0; nCnt < 10; nCnt++)
			{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
											0,
											2);//��
			}
		}

	}

	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexExplo(void)
{
	VERTEX_3D *pvtx;
	g_pD3DVtxBuffExplo->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(-E_SIZEWH / 2,E_SIZEWH / 2,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(E_SIZEWH / 2,E_SIZEWH / 2,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(-E_SIZEWH / 2,-E_SIZEWH / 2,0);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(E_SIZEWH / 2,-E_SIZEWH / 2,0);//���_�̍��W�i�[

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

g_pD3DVtxBuffExplo->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// �t���O�Z�b�g
//*****************************************************************************
void SetExplo(float x,float y,float z,float rot)
{
	for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{
		if(g_ExploData[nLoop].nFlag != 1)//�t���O�������Ă��Ȃ���
		{
			//�t���O�ƍ��W�Z�b�g
			g_ExploData[nLoop].nFlag = 1;
			g_ExploData[nLoop].nPosx = x;
			g_ExploData[nLoop].nPosy = y;
			g_ExploData[nLoop].nPosz = z;
			g_ExploData[nLoop].fSizex = 1.0f;
			g_ExploData[nLoop].fSizey = 1.0f;
			g_ExploData[nLoop].fSizez = 1.0f;
			g_ExploData[nLoop].fAlpha = 1.0f;
			g_ExploData[nLoop].nRot = rot;
			g_ExploData[nLoop].nCnt = 0;
			return;
		}
	}
}

D3DXVECTOR3 GetExploPos(int nNum)
{
	D3DXVECTOR3 vector = D3DXVECTOR3(g_ExploData[nNum].nPosx,g_ExploData[nNum].nPosy,g_ExploData[nNum].nPosz);
	return vector;
}

//End Of FIle