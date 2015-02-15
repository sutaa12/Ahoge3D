//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshroll.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "meshroll.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_FIELD ("data/TEXTURE/field001.jpg")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertexRoll(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureRoll;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffRoll;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBuffRoll;//�C���f�b�N�X�o�b�t�@

D3DXMATRIX g_mtxWorldRoll;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posRoll;						// �ʒu
D3DXVECTOR3 g_rotRoll;						// ����

int g_nNumRollBlockX, g_nNumRollBlockZ;				// �u���b�N��
int g_nNumRollVertex;							// �����_��
int g_nNumRollPolygon;							// ���|���S����
float g_fSizeRollBlockX, g_fSizeRollBlockZ;			// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshRoll(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//�t�B�[���h������
	g_posRoll = D3DXVECTOR3(100.0,0.0,100.0);
	g_rotRoll = D3DXVECTOR3(0.0,0.0,0.0);
	g_nNumRollBlockX = nNumBlockX;
	g_nNumRollBlockZ = nNumBlockZ;
	g_fSizeRollBlockX = fSizeBlockX;
	g_fSizeRollBlockZ = fSizeBlockZ;
	g_nNumRollVertex = (nNumBlockX+2)*(nNumBlockZ*2)-2;//�������o
	g_nNumRollPolygon = (nNumBlockX+1)*(nNumBlockZ+1);//�|���S����
	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*g_nNumRollPolygon,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffRoll,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}

	//�C���f�b�N�X���_���̐ݒ�
	if(FAILED(pDevice->CreateIndexBuffer(	//���_�o�b�t�@�쐬
				sizeof(WORD)*g_nNumRollVertex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_pD3DIndexBuffRoll,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}

	InitVertexRoll();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureRoll);
	return S_OK;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshRoll(void)
{
	if(g_pD3DVtxBuffRoll!=NULL)
	{
		g_pD3DVtxBuffRoll->Release();//������
		g_pD3DVtxBuffRoll=NULL;
	}
	if(g_pD3DTextureRoll!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureRoll->Release();
		g_pD3DTextureRoll=NULL;//�A�h���X�����
	}
	if(&g_pD3DIndexBuffRoll!=NULL)
	{
		g_pD3DIndexBuffRoll->Release();
		g_pD3DIndexBuffRoll=NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshRoll(void)
{

}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawMeshRoll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMatrixIdentity(&g_mtxWorldRoll);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldRoll,&g_mtxWorldRoll,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxWorldRoll,&g_mtxWorldRoll,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,-400.0f,0.0f,150.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldRoll,&g_mtxWorldRoll,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxWorldRoll);


	//�|���S���̕`��
	pDevice->SetStreamSource(0,g_pD3DVtxBuffRoll,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetIndices(g_pD3DIndexBuffRoll);

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureRoll);//�e�N�X�`���Z�b�g

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									0,
									g_nNumRollVertex,
									0,
									(g_nNumRollBlockX*g_nNumRollBlockZ)*2+((g_nNumRollBlockZ-1)*4));//��


}

//=============================================================================
// ���_�̏���������
//=============================================================================
void InitVertexRoll(void)
{

	VERTEX_3D *pvtx;
	
	g_pD3DVtxBuffRoll->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	int nNum=0;
	for(int nLoopZ=0;nLoopZ<=g_nNumRollBlockZ;nLoopZ++)//Z�u���b�N��+1�܂�
	{
		for(int nLoopX=0;nLoopX<=g_nNumRollBlockX;nLoopX++)//X�u���b�N��+1�܂�
		{
			pvtx[nNum].vtx = D3DXVECTOR3(	(cosf((D3DX_PI * 2 / g_nNumRollBlockX) * nLoopX)) * ( g_fSizeRollBlockX/(g_nNumRollBlockX)),
										(g_fSizeRollBlockZ/(g_nNumRollBlockZ)) * (g_nNumRollBlockZ-nLoopZ),
										(sinf((D3DX_PI * 2 / g_nNumRollBlockX) * nLoopX)) * ( g_fSizeRollBlockX/(g_nNumRollBlockX)) );//���_�̍��W�i�[
			pvtx[nNum].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//�@���̍��W�i�[
			pvtx[nNum].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
			pvtx[nNum].tex = D3DXVECTOR2((float)nLoopX,(float)nLoopZ);
			nNum++;
		}
	}
	g_pD3DVtxBuffRoll->Unlock();//���b�N�̉�����Y�ꂸ��

	WORD *pIndex;
	
	g_pD3DIndexBuffRoll->Lock(0,0,(void**)&pIndex,0);

	int nIndexNum = g_nNumRollBlockX+1;//�C���f�b�N�X���_�̈ʒu
	int nIndexZcnt = 0;//�㒸�_�̈ʒu
	int nIndexReturn = 0;//�܂�Ԃ���

	for(int nBuff = 0 ; nBuff < g_nNumRollVertex ; nBuff++)
	{
		pIndex[nBuff] = nIndexNum;//���̐ݒ�
		nBuff++;
		pIndex[nBuff] = nIndexZcnt;//��̐ݒ�
		nIndexNum++;
		nIndexZcnt++;
		if(nIndexNum-nIndexReturn*2%g_nNumRollBlockX + (g_nNumRollBlockX*nIndexReturn) == 0&&nBuff < g_nNumRollVertex)//��̒��_�����T�C�Y�Ŋ���؂���
		{
			nBuff++;
			pIndex[nBuff] = nIndexNum;//���̐ݒ�
			nBuff++;
			pIndex[nBuff] = nIndexZcnt;//��̐ݒ�
		}
	}
 	g_pD3DIndexBuffRoll->Unlock();//���b�N

}

//End Of File