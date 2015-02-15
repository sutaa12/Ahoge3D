//=============================================================================
//
// ���b�V���h�[���̏��� [meshdoom.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "meshdoom.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_SKY ("data/TEXTURE/sky.jpg")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexDoom(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureDoom;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffDoom;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBuffDoom;//�C���f�b�N�X�o�b�t�@

D3DXMATRIX g_mtxWorldDoom;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posDoom;						// �ʒu
D3DXVECTOR3 g_rotDoom;						// ����

int g_nNumDoomBlockX, g_nNumDoomBlockZ;				// �u���b�N��
int g_nNumDoomVertex;							// �����_��
int g_nNumDoomPolygon;							// ���|���S����
float g_fSizeDoomBlockX, g_fSizeDoomBlockZ;			// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshDoom(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//�t�B�[���h������
	g_posDoom = D3DXVECTOR3(100.0,0.0,100.0);
	g_rotDoom = D3DXVECTOR3(0.0,0.0,0.0);
	g_nNumDoomBlockX = nNumBlockX;
	g_nNumDoomBlockZ = nNumBlockZ;
	g_fSizeDoomBlockX = fSizeBlockX;
	g_fSizeDoomBlockZ = fSizeBlockZ;
	g_nNumDoomVertex = (nNumBlockX+2)*(nNumBlockZ*2)-2;//�������o
	g_nNumDoomPolygon = (nNumBlockX+1)*(nNumBlockZ+1);//�|���S����
	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*g_nNumDoomPolygon,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffDoom,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}

	//�C���f�b�N�X���_���̐ݒ�
	if(FAILED(pDevice->CreateIndexBuffer(	//���_�o�b�t�@�쐬
				sizeof(WORD)*g_nNumDoomVertex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_pD3DIndexBuffDoom,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}

	InitVertexDoom();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_SKY,
								&g_pD3DTextureDoom);
	return S_OK;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshDoom(void)
{
	if(g_pD3DVtxBuffDoom!=NULL)
	{
		g_pD3DVtxBuffDoom->Release();//������
		g_pD3DVtxBuffDoom=NULL;
	}
	if(g_pD3DTextureDoom!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureDoom->Release();
		g_pD3DTextureDoom=NULL;//�A�h���X�����
	}
	if(&g_pD3DIndexBuffDoom!=NULL)
	{
		g_pD3DIndexBuffDoom->Release();
		g_pD3DIndexBuffDoom=NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshDoom(void)
{
	g_rotDoom.y+=0.01f;
	if(g_rotDoom.y>D3DX_PI)
	{
		g_rotDoom.y = -D3DX_PI;
	}
}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawMeshDoom(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMatrixIdentity(&g_mtxWorldDoom);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldDoom,&g_mtxWorldDoom,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,g_rotDoom.y,g_rotDoom.x,g_rotDoom.z);//YXZ
	D3DXMatrixMultiply(&g_mtxWorldDoom,&g_mtxWorldDoom,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,0,0,0);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldDoom,&g_mtxWorldDoom,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxWorldDoom);


	//�|���S���̕`��
	pDevice->SetStreamSource(0,g_pD3DVtxBuffDoom,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetIndices(g_pD3DIndexBuffDoom);

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureDoom);//�e�N�X�`���Z�b�g

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									0,
									g_nNumDoomVertex,
									0,
									(g_nNumDoomBlockX*g_nNumDoomBlockZ)*2+((g_nNumDoomBlockZ-1)*2));//��


}

//=============================================================================
// ���_�̏���������
//=============================================================================
void InitVertexDoom(void)
{

	VERTEX_3D *pvtx;
	
	g_pD3DVtxBuffDoom->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	int nNum=0;
	for(int nLoopZ=0;nLoopZ<=g_nNumDoomBlockZ;nLoopZ++)//Z�u���b�N��+1�܂�
	{
		for(int nLoopX=0;nLoopX<=g_nNumDoomBlockX;nLoopX++)//X�u���b�N��+1�܂�
		{
			pvtx[nNum].vtx = D3DXVECTOR3(sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ))	* (cosf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) * ( g_fSizeDoomBlockX / (g_nNumDoomBlockX)),
										cosf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * ( g_fSizeDoomBlockZ / (g_nNumDoomBlockZ)),
										sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * (sinf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) * ( g_fSizeDoomBlockX / (g_nNumDoomBlockX)) );//���_�̍��W�i�[
			pvtx[nNum].nor = D3DXVECTOR3(sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ))	* (cosf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)/1),
										cosf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)/1),
										sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * (sinf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) /1);//�@���̍��W�i�[
			pvtx[nNum].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
			pvtx[nNum].tex = D3DXVECTOR2((sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ))	* (cosf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) / 1.f),
										sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * (sinf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)/1.f));
			nNum++;
		}
	}
	g_pD3DVtxBuffDoom->Unlock();//���b�N�̉�����Y�ꂸ��

	WORD *pIndex;
	
	g_pD3DIndexBuffDoom->Lock(0,0,(void**)&pIndex,0);
	int nIndexNum = g_nNumDoomBlockX+1;//�C���f�b�N�X���_�̈ʒu
	int nIndexZcnt = 0;//�㒸�_�̈ʒu
	int nIndexReturn = 0;//�܂�Ԃ���

	for(int nBuff = 0 ; nBuff < g_nNumDoomVertex ; nBuff++)
	{
		pIndex[nBuff] = nIndexNum;//���̐ݒ�
		nBuff++;
		pIndex[nBuff] = nIndexZcnt;//��̐ݒ�
		nIndexNum++;
		nIndexZcnt++;
		if(nIndexNum-nIndexReturn*2%g_nNumDoomBlockX + (g_nNumDoomBlockX*nIndexReturn) == 0&&nBuff < g_nNumDoomVertex)//��̒��_�����T�C�Y�Ŋ���؂���
		{
			nBuff++;
			pIndex[nBuff] = nIndexNum;//���̐ݒ�
			nBuff++;
			pIndex[nBuff] = nIndexZcnt;//��̐ݒ�
		}
	}
 	g_pD3DIndexBuffDoom->Unlock();//���b�N

}

//End Of File