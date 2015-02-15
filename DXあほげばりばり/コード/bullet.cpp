//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "main.h"
#include "explo.h"
#include "effect.h"
#include "score.h"
#include "enemy.h"
#include "hitchk.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_BULLET ("data/TEXTURE/bullet000.png")
#define BULEET_RADIUS (10)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
D3DXMATRIX GetMatrixView(void);
void InitVertexBullet(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxviewBullet;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojectionBullet;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworldBullet;		//���[���h�}�g���b�N�X


D3DXVECTOR3 g_posBullet;		//�n�ʂ̈ʒu
D3DXVECTOR3 g_rotBullet;		//�n�ʂ̌���
D3DXVECTOR3 g_sclBullet;		//�n�ʂ̑傫��

BULLETDATA g_BulletData[B_MAX];//�e�f�[�^�̔z��
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�B�[���h������
	g_posBullet = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotBullet = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclBullet = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffBullet,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexBullet();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureBullet);
	//�z�񏉊���
		for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{

		//�t���O�ƍ��W�Z�b�g
		g_BulletData[nLoop].nFlag = 0;
		g_BulletData[nLoop].fPosx = 0;
		g_BulletData[nLoop].fPosy = 0;
		g_BulletData[nLoop].fPosz = 0;
		g_BulletData[nLoop].nRot = 0;
		g_BulletData[nLoop].Col = 0;
		g_BulletData[nLoop].fPosCicle.Radius = BULEET_RADIUS;//���a�Z�b�g
		g_BulletData[nLoop].fPosCicle.Pos.Position = D3DXVECTOR3(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz);

	}
	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitBullet(void)
{
	if(g_pD3DVtxBuffBullet!=NULL)
	{
		g_pD3DVtxBuffBullet->Release();//������
		g_pD3DVtxBuffBullet=NULL;
	}
		if(g_pD3DTextureBullet!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateBullet(void)
{
	for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{
		if(g_BulletData[nLoop].nFlag == 1)
		{
			g_BulletData[nLoop].fPosx -= sinf(g_BulletData[nLoop].nRot)*B_SPEED;
			g_BulletData[nLoop].fPosz -= cosf(g_BulletData[nLoop].nRot)*B_SPEED;
			g_BulletData[nLoop].fPosCicle.Pos.Position = D3DXVECTOR3(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz);
			
			for(int nLoopEne = 0;nLoopEne < ENEMY_MAX;nLoopEne++)
			{
				if(GetEnemyLife(nLoopEne) == 1 && CircleHitChk(g_BulletData[nLoop].fPosCicle,GetEnemyPos(nLoopEne)))
				{
					SetEnemyLife(nLoopEne);//HP��ς���
				//�����Z�b�g
				SetExplo(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx-rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%30 - 20,g_BulletData[nLoop].fPosy-rand()%20,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				g_BulletData[nLoop].nFlag = 0;
				AddScore(20);
				}
			}
			//g_BulletData[nLoop].Col = ((g_BulletData[nLoop].Col >> 16)) - 1000;
			g_BulletData[nLoop].Col -= 100;
			SetEffect(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot,g_BulletData[nLoop].Col);
			
			//�t�B�[���h�̊O�ɏo����t���O��0�ɂ���
			if(g_BulletData[nLoop].fPosx < 0 || g_BulletData[nLoop].fPosx > FIELDSIZEMAX || g_BulletData[nLoop].fPosz < 0 || g_BulletData[nLoop].fPosz > FIELDSIZEMAX )
			{
				//�����Z�b�g
				SetExplo(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx-rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%30 - 20,g_BulletData[nLoop].fPosy-rand()%20,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				g_BulletData[nLoop].nFlag = 0;
				AddScore(20);
			}
		}
	}
}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;
	mtxView = GetMatrixView();//�J�����̃r���[���Ƃ��Ă���
	D3DXMatrixIdentity(&g_mtxworldBullet);//�{�[����Identey������
	D3DXMatrixInverse(&g_mtxworldBullet,NULL,&mtxView);//�t�s��
	g_mtxworldBullet._41 = 0.0f;
	g_mtxworldBullet._42 = 0.0f;
	g_mtxworldBullet._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,g_posBullet.x,g_posBullet.y,g_posBullet.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldBullet);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF,0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

	for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{
		if(g_BulletData[nLoop].nFlag == 1)//�t���O�������Ă��Ȃ���
		{
			g_posBullet = g_BulletData[nLoop].fPosCicle.Pos.Position;

	D3DXMatrixIdentity(&g_mtxworldBullet);//�{�[����Identey������
	D3DXMatrixInverse(&g_mtxworldBullet,NULL,&mtxView);//�t�s��
	g_mtxworldBullet._41 = 0.0f;
	g_mtxworldBullet._42 = 0.0f;
	g_mtxworldBullet._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,g_posBullet.x,g_posBullet.y,g_posBullet.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldBullet);

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldBullet);
			//�|���S���̕`��

			pDevice->SetStreamSource(0,g_pD3DVtxBuffBullet,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

			pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

			pDevice->SetTexture(0,g_pD3DTextureBullet);//�e�N�X�`���Z�b�g

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
											0,
											2);//��
		}

	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexBullet(void)
{
	VERTEX_3D *pvtx;
	g_pD3DVtxBuffBullet->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,B_SIZEWH,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(B_SIZEWH,B_SIZEWH,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(B_SIZEWH,0.0f,0);//���_�̍��W�i�[

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

g_pD3DVtxBuffBullet->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// �t���O�Z�b�g
//*****************************************************************************
void SetShot(float x,float y,float z,float rot)
{
	for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{
		if(g_BulletData[nLoop].nFlag != 1)//�t���O�������Ă��Ȃ���
		{
			//�t���O�ƍ��W�Z�b�g
			g_BulletData[nLoop].nFlag = 1;
			g_BulletData[nLoop].fPosx = x;
			g_BulletData[nLoop].fPosy = y;
			g_BulletData[nLoop].fPosz = z;
			g_BulletData[nLoop].nRot = rot;
			g_BulletData[nLoop].Col = D3DCOLOR_RGBA(rand()%255,rand()%255,rand()%255,0);
			return;
		}
	}
}

int GetBulletFlag(int nNum)
{
	return g_BulletData[nNum].nFlag;
}
D3DXVECTOR3 GetBulletPos(int nNum)
{
	D3DXVECTOR3 vector = D3DXVECTOR3(g_BulletData[nNum].fPosx,g_BulletData[nNum].fPosy,g_BulletData[nNum].fPosz);
	return vector;
}

//End Of File