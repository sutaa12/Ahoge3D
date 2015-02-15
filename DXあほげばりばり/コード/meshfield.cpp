//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEX_FIELD ("data/TEXTURE/field1.dds")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitVertex(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBuffField;//�C���f�b�N�X�o�b�t�@

D3DXMATRIX g_mtxWorldField;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posField;						// �ʒu
D3DXVECTOR3 g_rotField;						// ����

int g_nNumBlockX, g_nNumBlockZ;				// �u���b�N��
int g_nNumVertex;							// �����_��
int g_nNumPolygon;							// ���|���S����
float g_fSizeBlockX, g_fSizeBlockZ;			// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//�t�B�[���h������
	g_posField = D3DXVECTOR3(0.0,0.0,0.0);
	g_rotField = D3DXVECTOR3(0.0,0.0,0.0);
	g_nNumBlockX = nNumBlockX;
	g_nNumBlockZ = nNumBlockZ;
	g_fSizeBlockX = fSizeBlockX;
	g_fSizeBlockZ = fSizeBlockZ;
	g_nNumVertex = (nNumBlockX+2)*(nNumBlockZ*2)-2;//�������o
	g_nNumPolygon = (nNumBlockX+1)*(nNumBlockZ+1);//�|���S����
	//���_���̐ݒ�
	if(FAILED(pDevice->CreateVertexBuffer(	//���_�o�b�t�@�쐬
				sizeof(VERTEX_3D)*g_nNumPolygon,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffField,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}

	//�C���f�b�N�X���_���̐ݒ�
	if(FAILED(pDevice->CreateIndexBuffer(	//���_�o�b�t�@�쐬
				sizeof(WORD)*g_nNumVertex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_pD3DIndexBuffField,
				NULL)))
			{
		return E_FAIL;//�ǂݍ��߂Ȃ�������G���[
			}

	InitVertex();

	//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureField);
	return S_OK;


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if(g_pD3DVtxBuffField!=NULL)
	{
		g_pD3DVtxBuffField->Release();//������
		g_pD3DVtxBuffField=NULL;
	}
	if(g_pD3DTextureField!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureField->Release();
		g_pD3DTextureField=NULL;//�A�h���X�����
	}
	if(&g_pD3DIndexBuffField!=NULL)
	{
		g_pD3DIndexBuffField->Release();
		g_pD3DIndexBuffField=NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMatrixIdentity(&g_mtxWorldField);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldField,&g_mtxWorldField,&mtxScl);//�X�P�[���̔��f

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxWorldField,&g_mtxWorldField,&mtxRot);//��]�̔��f

	D3DXMatrixTranslation(&mtxTranslate,FIELDSIZEMAX/2,0.0f,FIELDSIZEMAX/2);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldField,&g_mtxWorldField,&mtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxWorldField);


	//�|���S���̕`��
	pDevice->SetStreamSource(0,g_pD3DVtxBuffField,0,sizeof(VERTEX_3D));//�|���S���Z�b�g

	pDevice->SetIndices(g_pD3DIndexBuffField);

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0,g_pD3DTextureField);//�e�N�X�`���Z�b�g

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̐ݒ�
									0,
									0,
									g_nNumVertex,
									0,
									(g_nNumBlockX*g_nNumBlockZ)*2+((g_nNumBlockZ-1)*2));//��


}

//=============================================================================
// ���_�̏���������
//=============================================================================
void InitVertex(void)
{

	VERTEX_3D *pvtx;
	
	g_pD3DVtxBuffField->Lock(0,0,(void**)&pvtx,0);//���_�̈ʒu�����b�N
	int nNum=0;
	for(int nLoopZ=0;nLoopZ<=g_nNumBlockZ;nLoopZ++)//Z�u���b�N��+1�܂�
	{
		for(int nLoopX=0;nLoopX<=g_nNumBlockX;nLoopX++)//X�u���b�N��+1�܂�
		{
			pvtx[nNum].vtx = D3DXVECTOR3((	g_fSizeBlockX/(g_nNumBlockX))*nLoopX - (g_fSizeBlockX / 2),
										0.0f,
										(g_fSizeBlockZ/(g_nNumBlockZ))*(g_nNumBlockZ-nLoopZ) - (g_fSizeBlockZ / 2));//���_�̍��W�i�[
			pvtx[nNum].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//�@���̍��W�i�[
			pvtx[nNum].diffuse = D3DCOLOR_RGBA(255,255,255,255); //�F�ݒ�
			pvtx[nNum].tex = D3DXVECTOR2((float)nLoopX,(float)nLoopZ);
			nNum++;
		}
	}
	g_pD3DVtxBuffField->Unlock();//���b�N�̉�����Y�ꂸ��

	WORD *pIndex;
	
	g_pD3DIndexBuffField->Lock(0,0,(void**)&pIndex,0);

	int nIndexNum = g_nNumBlockX+1;//�C���f�b�N�X���_�̈ʒu
	int nIndexZcnt = 0;//�㒸�_�̈ʒu
	int nIndexReturn = 0;//�܂�Ԃ���

	for(int nBuff = 0 ; nBuff < g_nNumVertex ; nBuff++)
	{
		pIndex[nBuff] = nIndexNum;//���̐ݒ�
		nBuff++;
		pIndex[nBuff] = nIndexZcnt;//��̐ݒ�
		nIndexNum++;
		nIndexZcnt++;
		if(nIndexNum-nIndexReturn*2%g_nNumBlockX + (g_nNumBlockX*nIndexReturn) == 0&&nBuff < g_nNumVertex)//��̒��_�����T�C�Y�Ŋ���؂���
		{
			nBuff++;
			pIndex[nBuff] = nIndexNum;//���̐ݒ�
			nBuff++;
			pIndex[nBuff] = nIndexZcnt;//��̐ݒ�
		}
	}
 	g_pD3DIndexBuffField->Unlock();//���b�N

}

//End Of File