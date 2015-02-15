//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
// マクロ定義
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
// プロトタイプ宣言
//*****************************************************************************
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureTitlePlayer;		// テクスチャへのポインタ
PLAYERDATA TitlePlayerData;
CAllocateHierarchy g_cTitlePlayer;

D3DXMATRIX g_TPmtxviewM;		//ビューマトリックス
D3DXMATRIX g_TPmtxprojectionM;	//プロジェクションマトリックス
D3DXMATRIX g_TPmtxworldM;		//ワールドマトリックス

LPD3DXMESH g_pD3DXMeshTitlePlayer;				//メッシュ情報へのポインタ
LPD3DXBUFFER g_pD3DXBuffMatTitlePlayer;			//マテリアル情報へのポインタ
DWORD g_nNumMatTitlePlayer;						//マテリアル情報数
float g_PosTitlePlayerSpeedX;						//モデルの加速度
float g_PosTitlePlayerSpeedZ;						//モデルの加速度
CHighLevelAnimController g_TPanim;
D3DXVECTOR3 g_PosTitlePlayerSpeed;

//=============================================================================
// 初期化処理
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
// 終了処理
//=============================================================================
void UninitTitlePlayer(void)
{
	if(g_pD3DXBuffMatTitlePlayer!=NULL)
	{
		g_pD3DXBuffMatTitlePlayer->Release();//初期化
		g_pD3DXBuffMatTitlePlayer=NULL;
	}

	if(g_pD3DXMeshTitlePlayer!=NULL)
	{
		g_pD3DXMeshTitlePlayer->Release();//初期化
		g_pD3DXMeshTitlePlayer=NULL;
	}

		if(g_pD3DTextureTitlePlayer!=NULL)//空でないなら
	{
		g_pD3DTextureTitlePlayer->Release();
		g_pD3DTextureTitlePlayer=NULL;//アドレスを空に
	}

	g_TPanim.Release();
	g_cTitlePlayer.Release();
		
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitlePlayer(void)
{
	g_TPanim.ChangeAnimation(3);
	g_TPanim.AdvanceTime(0.008f);
	
	#ifdef _DEBUG
	PrintDebugProc("モデル座標(X:%.2fY:%.2fZ:%.2f)\n",TitlePlayerData.fPosCicle.Pos.Position.x,TitlePlayerData.fPosCicle.Pos.Position.y,TitlePlayerData.fPosCicle.Pos.Position.z);
	PrintDebugProc("モデル向き(%.3f),速度(X:%.2f,Z:%.2f)\n",TitlePlayerData.fPosCicle.Pos.Rot.y,TitlePlayerData.fPosCicle.Pos.PositionDest.x,TitlePlayerData.fPosCicle.Pos.PositionDest.z);
	
	#endif
}



//=============================================================================
// 描画処理
//=============================================================================
void DrawTitlePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;	//デフォルトマテリアル
	D3DMATERIAL9 matDef;	//退避用マテリアル
	D3DXMATRIX TPmtxScl,TPmtxRot,TPmtxTranslate;


	//ブロジェクトマトリックス
	D3DXMatrixIdentity(&g_TPmtxworldM);
	D3DXMatrixScaling(&TPmtxScl,0.2f,0.2f,0.2f);//XYZ
	D3DXMatrixMultiply(&g_TPmtxworldM,&g_TPmtxworldM,&TPmtxScl);//スケールの反映
	//ワールドマトリックス
	D3DXMatrixRotationYawPitchRoll(&TPmtxRot,TitlePlayerData.fPosCicle.Pos.Rot.y,TitlePlayerData.fPosCicle.Pos.Rot.x,TitlePlayerData.fPosCicle.Pos.Rot.z);//YXZ
	D3DXMatrixMultiply(&g_TPmtxworldM,&g_TPmtxworldM,&TPmtxRot);//回転の反映

	D3DXMatrixTranslation(&TPmtxTranslate,TitlePlayerData.fPosCicle.Pos.Position.x,TitlePlayerData.fPosCicle.Pos.Position.y,TitlePlayerData.fPosCicle.Pos.Position.z);//XYZ
	D3DXMatrixMultiply(&g_TPmtxworldM,&g_TPmtxworldM,&TPmtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_TPmtxworldM);

	g_cTitlePlayer.SetWorld(&g_TPmtxworldM);
	pDevice->SetTexture(0,g_pD3DTextureTitlePlayer);

	g_cTitlePlayer.Draw(pDevice);
}
//End Of FIle