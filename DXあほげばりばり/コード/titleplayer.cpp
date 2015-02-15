//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "player.h"
#include "titleplayer.h"
#include "input.h"
#include "bullet.h"
#include "wall.h"
#include "effect.h"
#include "camera.h"
#include "scene.h"
#include "xallocatehierachy.h"
#include "animation.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLEPLAYER_MODEL ("data/MODEL/sintea.x")
#define TITLEPLAYER_TEX ("data/TEXTURE/playerskin.bmp")
#define X_POS_TP (FIELDSIZEMAX/2)
#define Y_POS_TP (30)
#define Z_POS_TP (FIELDSIZEMAX/2)
#define X_ROT (0.0f)
#define Y_ROT (0.0f)
#define Z_ROT (0.0f)
#define MOVE_TP_ROT (0.1f)
#define MOVE_TP_SPEED (0.1f)
#define MOVE_TP_POS_TP (5)
#define MOVE_TP_POS_TPSPEED (0.08f)

#define TITLEPLAYER_RADIUS (30)
#define TITLEPLAYER_INRAD (500)
#define TITLEPLAYER_OUTRAD (200)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureTitlePlayer;		// �e�N�X�`���ւ̃|�C���^
PLAYERDATA TitlePlayerData;
CAllocateHierarchy g_cTitlePlayer;

D3DXMATRIX g_TPmtxviewM;		//�r���[�}�g���b�N�X
D3DXMATRIX g_TPmtxprojectionM;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_TPmtxworldM;		//���[���h�}�g���b�N�X

LPD3DXMESH g_pD3DXMeshTitlePlayer;				//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pD3DXBuffMatTitlePlayer;			//�}�e���A�����ւ̃|�C���^
DWORD g_nNumMatTitlePlayer;						//�}�e���A�����
float g_PosTitlePlayerSpeedX;						//���f���̉����x
float g_PosTitlePlayerSpeedZ;						//���f���̉����x
CHighLevelAnimController g_TPanim;
D3DXVECTOR3 g_PosTitlePlayerSpeed;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitlePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	TitlePlayerData.fPosCicle.Pos.Rot = D3DXVECTOR3(0.0f,0.5f,0.0f);
	TitlePlayerData.fPosCicle.Pos.RotDest = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
	TitlePlayerData.fPosCicle.Pos.Position = D3DXVECTOR3(40,Y_POS_TP-60,150);
	TitlePlayerData.fPosCicle.Pos.PositionDest = D3DXVECTOR3(0,Y_POS_TP-50,200);
	g_PosTitlePlayerSpeed.x = 0;
	g_PosTitlePlayerSpeed.z = 0;
	g_PosTitlePlayerSpeed = D3DXVECTOR3(X_POS_TP,Y_POS_TP,Z_POS_TP);
	TitlePlayerData.fPosCicle.Pos.Col = 0;
	TitlePlayerData.fPosCicle.Pos.nLife = 1;
	TitlePlayerData.AttackFlag = 0;
	TitlePlayerData.fINRad = TITLEPLAYER_INRAD;
	TitlePlayerData.fOUTRad = TITLEPLAYER_OUTRAD;
	
	g_cTitlePlayer.Load(pDevice,TITLEPLAYER_MODEL,TITLEPLAYER_TEX);
	g_TPanim.SetAnimationController(g_cTitlePlayer.GetAnimeCtrl());

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitlePlayer(void)
{
	if(g_pD3DXBuffMatTitlePlayer!=NULL)
	{
		g_pD3DXBuffMatTitlePlayer->Release();//������
		g_pD3DXBuffMatTitlePlayer=NULL;
	}

	if(g_pD3DXMeshTitlePlayer!=NULL)
	{
		g_pD3DXMeshTitlePlayer->Release();//������
		g_pD3DXMeshTitlePlayer=NULL;
	}

		if(g_pD3DTextureTitlePlayer!=NULL)//��łȂ��Ȃ�
	{
		g_pD3DTextureTitlePlayer->Release();
		g_pD3DTextureTitlePlayer=NULL;//�A�h���X�����
	}

	g_TPanim.Release();
	g_cTitlePlayer.Release();
		
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitlePlayer(void)
{
	g_TPanim.ChangeAnimation(3);
	g_TPanim.AdvanceTime(0.008f);
	
	#ifdef _DEBUG
	PrintDebugProc("���f�����W(X:%.2fY:%.2fZ:%.2f)\n",TitlePlayerData.fPosCicle.Pos.Position.x,TitlePlayerData.fPosCicle.Pos.Position.y,TitlePlayerData.fPosCicle.Pos.Position.z);
	PrintDebugProc("���f������(%.3f),���x(X:%.2f,Z:%.2f)\n",TitlePlayerData.fPosCicle.Pos.Rot.y,TitlePlayerData.fPosCicle.Pos.PositionDest.x,TitlePlayerData.fPosCicle.Pos.PositionDest.z);
	
	#endif
}



//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitlePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;	//�f�t�H���g�}�e���A��
	D3DMATERIAL9 matDef;	//�ޔ�p�}�e���A��
	D3DXMATRIX TPmtxScl,TPmtxRot,TPmtxTranslate;


	//�u���W�F�N�g�}�g���b�N�X
	D3DXMatrixIdentity(&g_TPmtxworldM);
	D3DXMatrixScaling(&TPmtxScl,0.2f,0.2f,0.2f);//XYZ
	D3DXMatrixMultiply(&g_TPmtxworldM,&g_TPmtxworldM,&TPmtxScl);//�X�P�[���̔��f
	//���[���h�}�g���b�N�X
	D3DXMatrixRotationYawPitchRoll(&TPmtxRot,TitlePlayerData.fPosCicle.Pos.Rot.y,TitlePlayerData.fPosCicle.Pos.Rot.x,TitlePlayerData.fPosCicle.Pos.Rot.z);//YXZ
	D3DXMatrixMultiply(&g_TPmtxworldM,&g_TPmtxworldM,&TPmtxRot);//��]�̔��f

	D3DXMatrixTranslation(&TPmtxTranslate,TitlePlayerData.fPosCicle.Pos.Position.x,TitlePlayerData.fPosCicle.Pos.Position.y,TitlePlayerData.fPosCicle.Pos.Position.z);//XYZ
	D3DXMatrixMultiply(&g_TPmtxworldM,&g_TPmtxworldM,&TPmtxTranslate);//���[���h�̔��f

	pDevice->SetTransform(D3DTS_WORLD,&g_TPmtxworldM);

	g_cTitlePlayer.SetWorld(&g_TPmtxworldM);
	pDevice->SetTexture(0,g_pD3DTextureTitlePlayer);

	g_cTitlePlayer.Draw(pDevice);
}
//End Of FIle