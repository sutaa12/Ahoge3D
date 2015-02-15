//=============================================================================
//
// カメラ処理 [Titlecamera.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "titlecamera.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CT_MOVEX (1)
#define CT_MOVEY (1)
#define CT_MOVEZ (1)
#define CT_ROTMOVE (0.01f)
#define CT_POSY (60)
#define CT_MODELPOS (160)
#define CT_SPEED (0.15f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3 g_posTitlecameraP;	//カメラの位置
D3DXVECTOR3 g_posTitlecameraR;	//カメラの注視点
D3DXVECTOR3 g_vecTitlecameraV;	//カメラの方向ベクトル

D3DXVECTOR3 g_posTitlecameraPDest;	//目的のカメラの位置
D3DXVECTOR3 g_posTitlecameraRDest;	//目的のカメラの注視点

D3DXMATRIX g_tcmtxview;		//ビューマトリックス
D3DXMATRIX g_tcmtxprojection;	//プロジェクションマトリックス
D3DXMATRIX g_tcmtxworld;		//ワールドマトリックス


float g_vecTitlecamera;	//カメラ注視点までの距離
D3DXVECTOR3 g_rotTitlecamera;	//カメラの向き


//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT InitTitleCamera(void)
{
	g_rotTitlecamera = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_posTitlecameraP = D3DXVECTOR3(0.0f,0,0);
	g_posTitlecameraR = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,-5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);
	g_vecTitlecameraV = D3DXVECTOR3(0.0f,1.0f,0.0f);

	g_vecTitlecamera = 100;
	//カメラの位置の差分設定	
	g_posTitlecameraPDest = D3DXVECTOR3(0.0f,CT_POSY,50.0f);
	//カメラの回転の差分設定
	g_posTitlecameraRDest = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);
	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitTitleCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateTitleCamera(void)
{
	g_rotTitlecamera = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_posTitlecameraP = D3DXVECTOR3(0.0f,0,0);
	g_posTitlecameraR = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,-5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);
	g_vecTitlecameraV = D3DXVECTOR3(0.0f,1.0f,0.0f);

	g_vecTitlecamera = 100;
	//カメラの位置の差分設定	
	g_posTitlecameraPDest = D3DXVECTOR3(0.0f,CT_POSY,50.0f);
	//カメラの回転の差分設定
	g_posTitlecameraRDest = D3DXVECTOR3(g_posTitlecameraP.x+sinf(g_rotTitlecamera.y)*g_vecTitlecamera,5.0f,g_posTitlecameraP.z+cosf(g_rotTitlecamera.y)*g_vecTitlecamera);

	#ifdef _DEBUG
	PrintDebugProc("カメラ座標(X:%.2fY:%.2fZ:%.2f)\n",g_posTitlecameraP.x,g_posTitlecameraP.y,g_posTitlecameraP.z);
	PrintDebugProc("カメラ注視点(X:%.2fY:%.2fZ:%.2f)\n",g_posTitlecameraR.x,g_posTitlecameraR.y,g_posTitlecameraR.z);
	PrintDebugProc("カメラ向き(%.3f),速度(X:%.2f,Z:%.2f)\n",g_rotTitlecamera.y,g_posTitlecameraRDest.x,g_posTitlecameraRDest.z);
	#endif

}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetTitleCamera(void)
{
	D3DXMATRIX tcmtxScl,tcmtxRot,tcmtxTranslate;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//ビューマトリックス
	D3DXMatrixIdentity(&g_tcmtxview);
	D3DXMatrixLookAtLH(&g_tcmtxview,&g_posTitlecameraP,&g_posTitlecameraR,&g_vecTitlecameraV);
	pDevice->SetTransform(D3DTS_VIEW,&g_tcmtxview);
	D3DXMatrixIdentity(&g_tcmtxprojection);
	D3DXMatrixPerspectiveFovLH(	&g_tcmtxprojection,//マトリックス作成
								(D3DX_PI/4),//視野
								(float)SCREEN_WIDTH/SCREEN_HEIGHT,//アスペクト比
								10.0f,	//距離
								10000.0f);//描画距離
	pDevice->SetTransform(D3DTS_PROJECTION,&g_tcmtxprojection);

		//ブロジェクトマトリックス
	D3DXMatrixIdentity(&g_tcmtxworld);
	D3DXMatrixScaling(&tcmtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_tcmtxworld,&g_tcmtxworld,&tcmtxScl);//スケールの反映

}

//End Of File