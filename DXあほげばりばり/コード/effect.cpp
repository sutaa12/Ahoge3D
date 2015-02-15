//=============================================================================
//
// �e�̏��� [effect.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "effect.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_EFFECT ("data/TEXTURE/effect.dds")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
D3DXMATRIX GetMatrixView(void);
void InitVertexEffect(void);
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxviewEffect;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojectionEffect;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworldEffect;		//���[���h�}�g���b�N�X


D3DXVECTOR3 g_posEffect;		//�n�ʂ̈ʒu
D3DXVECTOR3 g_rotEffect;		//�n�ʂ̌���
D3DXVECTOR3 g_sclEffect;		//�n�ʂ̑傫��

EFFECTDATA g_EffectData[EF_MAX];//�e�f�[�^�̔z��
//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�B�[���h������
	g_posEffect = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotEffect = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclEffect = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffEffect,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}
	InitVertexEffect();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_EFFECT,
								&g_pD3DTextureEffect);
	//�z�񏉊���
		for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{

		//�t���O�ƍ��W�Z�b�g
		g_EffectData[nLoop].nFlag = 0;
		g_EffectData[nLoop].nPosx = 0;
		g_EffectData[nLoop].nPosy = 0;
		g_EffectData[nLoop].nPosz = 0;
		//�傫���Z�b�g
		g_EffectData[nLoop].fSizex = 1.0f;
		g_EffectData[nLoop].fSizey = 1.0f;
		g_EffectData[nLoop].fSizez = 1.0f;
		//�F�ݒ�
		g_EffectData[nLoop].fAlpha = 1.0f;
		g_EffectData[nLoop].nRot = 0;
		g_EffectData[nLoop].dCol = 0;

	}
	return S_OK;
}
//*****************************************************************************
// �I������
//*****************************************************************************
void UninitEffect(void)
{
	if(g_pD3DVtxBuffEffect!=NULL)
	{
		g_pD3DVtxBuffEffect->Release();//������
		g_pD3DVtxBuffEffect=NULL;
	}
		if(g_pD3DTextureEffect!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect=NULL;//�A�h���X�����
	}
}
//*****************************************************************************
// �X�V����
//*****************************************************************************
void UpdateEffect(void)
{
	for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{
		if(g_EffectData[nLoop].nFlag == 1)
		{

			int r,g,b;
			//�F�ݒ�
			r = ((g_EffectData[nLoop].dCol >> 0) & 0xff);
			g = ((g_EffectData[nLoop].dCol >> 8) & 0xff);
			b = ((g_EffectData[nLoop].dCol >> 16) & 0xff);
			g_EffectData[nLoop].fAlpha -= EF_ALPHASPEED;//�T�C�Y�ύX
			//�F�ύX
			g_EffectData[nLoop].dCol = D3DCOLOR_RGBA(r,g,b,(int)(g_EffectData[nLoop].fAlpha * 255.f));

		//�A�j���[�V�����J�E���g�Z�b�g
			g_EffectData[nLoop].nCnt++;

			//����0�ɂȂ�����0�ɂ���
			if(g_EffectData[nLoop].fAlpha < 0)
			{
				g_EffectData[nLoop].nFlag = 0;
			}
		}
	}
}
//*****************************************************************************
// �`�揈��
//*****************************************************************************
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;

	
		//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);	


	mtxView = GetMatrixView();//�J�����̃r���[���Ƃ��Ă���

	D3DXMatrixIdentity(&g_mtxworldEffect);//�{�[����Identey������
	D3DXMatrixInverse(&g_mtxworldEffect,NULL,&mtxView);//�t�s��
	g_mtxworldEffect._41 = 0.0f;
	g_mtxworldEffect._42 = 0.0f;
	g_mtxworldEffect._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,g_posEffect.x,g_posEffect.y,g_posEffect.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldEffect);

	for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{
		if(g_EffectData[nLoop].nFlag == 1)//�t���O�������Ă��Ȃ���
		{
				VERTEX_3D *pvtx;
				g_pD3DVtxBuffEffect->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
				
				for(int loop = 0;loop<4;loop++)
				{
				pvtx[loop].diffuse = g_EffectData[nLoop].dCol; //�F�ݒ�
				}
				pvtx[0].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f,0.0f);
				pvtx[1].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f + 1.0f/4,0.0f);
				pvtx[2].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f,1.0f);
				pvtx[3].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f + 1.0f/4,1.0f);

				g_pD3DVtxBuffEffect->Unlock();//���b�N�̉�����Y�ꂸ��


			D3DXMatrixIdentity(&g_mtxworldEffect);//�{�[����Identey������
			D3DXMatrixInverse(&g_mtxworldEffect,NULL,&mtxView);//�t�s��
			g_mtxworldEffect._41 = 0.0f;
			g_mtxworldEffect._42 = 0.0f;
			g_mtxworldEffect._43 = 0.0f;

			g_posEffect = D3DXVECTOR3(g_EffectData[nLoop].fSizex,g_EffectData[nLoop].fSizey,g_EffectData[nLoop].fSizez);

			D3DXMatrixScaling(&mtxScl,g_posEffect.x,g_posEffect.y,g_posEffect.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxScl);//�X�P�[���̔��f

			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
			D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxRot);//��]�̔��f

			g_posEffect = D3DXVECTOR3(	g_EffectData[nLoop].nPosx,
									g_EffectData[nLoop].nPosy,
									g_EffectData[nLoop].nPosz);

			D3DXMatrixTranslation(&mtxTranslate,g_posEffect.x,g_posEffect.y,g_posEffect.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxTranslate);//���[���h�̔��f

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldEffect);

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldEffect);
			//�|���S���̕`��

			pDevice->SetStreamSource(0,g_pD3DVtxBuffEffect,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

			pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

			pDevice->SetTexture(0,g_pD3DTextureEffect);//�e�N�X�`���Z�b�g

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
											0,
											2);//��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
											0,
											2);//��
		}

	}
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			//�e�̃Z�b�g�@���Z�����쐬
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
}
//*****************************************************************************
// ���_������
//*****************************************************************************
void InitVertexEffect(void)
{
	VERTEX_3D *pvtx;
	g_pD3DVtxBuffEffect->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N

				//���_���̏�����
	pvtx[0].vtx = D3DXVECTOR3(0,EF_SIZEWH,0.0f);//���_�̍��W�i�[
	pvtx[1].vtx = D3DXVECTOR3(EF_SIZEWH,EF_SIZEWH,0.0f);//���_�̍��W�i�[
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//���_�̍��W�i�[
	pvtx[3].vtx = D3DXVECTOR3(EF_SIZEWH,0.0f,0);//���_�̍��W�i�[

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,100,0,255); //�F�ݒ�

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f/4,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f/4,1.0f);

g_pD3DVtxBuffEffect->Unlock();//���b�N�̉�����Y�ꂸ��

}
//*****************************************************************************
// �t���O�Z�b�g
//*****************************************************************************
void SetEffect(float x,float y,float z,float rot,D3DCOLOR col)
{
	for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{
		if(g_EffectData[nLoop].nFlag != 1)//�t���O�������Ă��Ȃ���
		{
			//�t���O�ƍ��W�Z�b�g
			g_EffectData[nLoop].nFlag = 1;
			g_EffectData[nLoop].nPosx = x;
			g_EffectData[nLoop].nPosy = y;
			g_EffectData[nLoop].nPosz = z;
			g_EffectData[nLoop].fSizex = 1.0f;
			g_EffectData[nLoop].fSizey = 1.0f;
			g_EffectData[nLoop].fSizez = 1.0f;
			g_EffectData[nLoop].fAlpha = 1.0f;
			g_EffectData[nLoop].nRot = rot;
			g_EffectData[nLoop].nCnt = 0;
			g_EffectData[nLoop].dCol = col;
			return;
		}
	}
}

D3DXVECTOR3 GetEffectPos(int nNum)
{
	D3DXVECTOR3 vector = D3DXVECTOR3(g_EffectData[nNum].nPosx,g_EffectData[nNum].nPosy,g_EffectData[nNum].nPosz);
	return vector;
}

//End Of File