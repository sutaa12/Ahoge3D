//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "camera.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define C_MOVEX (1)
#define C_MOVEY (1)
#define C_MOVEZ (1)
#define C_ROTMOVE (0.01f)
#define C_POSY (80)
#define C_MODELPOS (160)
#define C_SPEED (0.15f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3 g_poscameraP;	//カメラの位置
D3DXVECTOR3 g_poscameraR;	//カメラの注視点
D3DXVECTOR3 g_veccameraV;	//カメラの方向ベクトル

D3DXVECTOR3 g_poscameraPDest;	//目的のカメラの位置
D3DXVECTOR3 g_poscameraRDest;	//目的のカメラの注視点

D3DXMATRIX g_mtxview;		//ビューマトリックス
D3DXMATRIX g_mtxprojection;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworld;		//ワールドマトリックス


float g_veccamera;	//カメラ注視点までの距離
D3DXVECTOR3 g_rotcamera;	//カメラの向き


//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT InitCamera(void)
{
	D3DXVECTOR3 PlayerPos = GetPlayerPosition();

	g_rotcamera = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//モデルの位置からZ方向に一定距離
	g_poscameraP = D3DXVECTOR3(	PlayerPos.x,
									PlayerPos.y+C_POSY,
									PlayerPos.z-C_MODELPOS);
	//モデルの前方の位置
	g_poscameraR = D3DXVECTOR3(	PlayerPos.x-sinf(PlayerRot().y)*g_veccamera,
									PlayerPos.y,
									PlayerPos.z+cosf(PlayerRot().y)*g_veccamera);

	//カメラの法線設定
	g_veccameraV = D3DXVECTOR3(0.0f,1.0f,0.0f);
	//カメラの注視点までの距離設定
	g_veccamera = 5;
	g_poscameraR = D3DXVECTOR3(g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera,5.0f,g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera);
	g_mtxview ;		//ビューマトリックス
	g_mtxprojection;	//プロジェクションマトリックス
	g_mtxworld;		//ワールドマトリックス
	//カメラの位置の差分設定	
	g_poscameraPDest = D3DXVECTOR3(0.0f,C_POSY,50.0f);
	//カメラの回転の差分設定
	g_poscameraRDest = D3DXVECTOR3(g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera,5.0f,g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera);
	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	
	//モデルの位置取得
	D3DXVECTOR3 PlayerPos = GetPlayerPosition();


		if(GetKeyboardPress(DIK_SPACE))
	{
		if(GetKeyboardTrigger(DIK_SPACE))
		{
		g_poscameraPDest = D3DXVECTOR3(	g_poscameraRDest.x + sinf(g_rotcamera.y)* (C_MODELPOS*1.5),
									g_poscameraRDest.y+C_POSY*3,
										g_poscameraRDest.z + cosf(g_rotcamera.y) * (C_MODELPOS*1.5f));
		}
		if(GetKeyboardPress(DIK_Q))
	{
		g_rotcamera.y+= D3DX_PI /90;
		g_poscameraPDest = D3DXVECTOR3(	g_poscameraRDest.x + sinf(g_rotcamera.y)* (C_MODELPOS*1.5),
								g_poscameraRDest.y+C_POSY*2,
									g_poscameraRDest.z + cosf(g_rotcamera.y) * (C_MODELPOS*1.5f));
	}else
	if(GetKeyboardPress(DIK_E))
	{
	//モデルの位置からZ方向に一定距離
	g_rotcamera.y-= D3DX_PI /90;
	g_poscameraPDest = D3DXVECTOR3(	g_poscameraRDest.x + sinf(g_rotcamera.y)* (C_MODELPOS*1.5),
								g_poscameraRDest.y+C_POSY*2,
									g_poscameraRDest.z + cosf(g_rotcamera.y) * (C_MODELPOS*1.5));
	}
	//モデルの位置からZ方向に一定距離
	g_poscameraP = D3DXVECTOR3(	g_poscameraP.x+(g_poscameraPDest.x-g_poscameraP.x)*C_SPEED,
								g_poscameraP.y+(g_poscameraPDest.y-g_poscameraP.y)*C_SPEED,
								g_poscameraP.z+(g_poscameraPDest.z-g_poscameraP.z)*C_SPEED);


		}else{
	//モデルの前方の位置
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*C_SPEED,
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*C_SPEED);
	if(GetKeyboardPress(DIK_W)||GetKeyboardPress(DIK_A)||GetKeyboardPress(DIK_D)||GetKeyboardPress(DIK_S))
	{
	//モデルの前方の位置
	g_poscameraRDest = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*(C_SPEED*1.5f),
									g_poscameraR.y,
									g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*(C_SPEED*1.5f));
		//モデルの前方の位置
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*(C_SPEED*1.5f),
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*(C_SPEED*1.5f));

	}
	
	//モデルの加速度取得
	D3DXVECTOR3 Playerspeed = GetPlayerSpeed();
	g_rotcamera.y = PlayerRot().y;

	//モデルの位置からZ方向に一定距離
	g_poscameraPDest = D3DXVECTOR3(	PlayerPos.x - sinf(PlayerRot().y)* (-C_MODELPOS),
								PlayerPos.y+C_POSY,
									PlayerPos.z - cosf(PlayerRot().y) * (-C_MODELPOS));
	//モデルの前方の位置
	g_poscameraRDest = D3DXVECTOR3(	PlayerPos.x-sinf(PlayerRot().y)*g_veccamera,
									PlayerPos.y,
									PlayerPos.z-cosf(PlayerRot().y)*g_veccamera);

		//モデルの位置からZ方向に一定距離
	g_poscameraP = D3DXVECTOR3(	g_poscameraP.x+(g_poscameraPDest.x-g_poscameraP.x)*C_SPEED,
								g_poscameraP.y+(g_poscameraPDest.y-g_poscameraP.y)*C_SPEED,
								g_poscameraP.z+(g_poscameraPDest.z-g_poscameraP.z)*C_SPEED);

	//モデルの前方の位置
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*C_SPEED,
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*C_SPEED);
	if(GetKeyboardPress(DIK_W)||GetKeyboardPress(DIK_A)||GetKeyboardPress(DIK_D)||GetKeyboardPress(DIK_S))
	{
	//モデルの前方の位置
	g_poscameraRDest = D3DXVECTOR3(	PlayerPos.x-sinf(PlayerRot().y)*g_veccamera+Playerspeed.x,
									PlayerPos.y,
									PlayerPos.z-cosf(PlayerRot().y)*g_veccamera+Playerspeed.z);
		//モデルの前方の位置
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*(C_SPEED*1.5f),
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*(C_SPEED*1.5f));
	}
	g_rotcamera.y = PlayerRot().y;


	}
	if(g_rotcamera.y>=D3DX_PI||g_rotcamera.y<-D3DX_PI)
	{
		g_rotcamera.y*=-1;
	}

	#ifdef _DEBUG
	PrintDebugProc("カメラ座標(X:%.2fY:%.2fZ:%.2f)\n",g_poscameraP.x,g_poscameraP.y,g_poscameraP.z);
	PrintDebugProc("カメラ注視点(X:%.2fY:%.2fZ:%.2f)\n",g_poscameraR.x,g_poscameraR.y,g_poscameraR.z);
	PrintDebugProc("カメラ向き(%.3f),速度(X:%.2f,Z:%.2f)\n",g_rotcamera.y,g_poscameraRDest.x,g_poscameraRDest.z);
	#endif

}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//ビューマトリックス
	D3DXMatrixIdentity(&g_mtxview);
	D3DXMatrixLookAtLH(&g_mtxview,&g_poscameraP,&g_poscameraR,&g_veccameraV);
	pDevice->SetTransform(D3DTS_VIEW,&g_mtxview);
	D3DXMatrixIdentity(&g_mtxprojection);
	D3DXMatrixPerspectiveFovLH(	&g_mtxprojection,//マトリックス作成
								(D3DX_PI/4),//視野
								(float)SCREEN_WIDTH/SCREEN_HEIGHT,//アスペクト比
								10.0f,	//距離
								10000.0f);//描画距離
	pDevice->SetTransform(D3DTS_PROJECTION,&g_mtxprojection);

		//ブロジェクトマトリックス
	D3DXMatrixIdentity(&g_mtxworld);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworld,&g_mtxworld,&mtxScl);//スケールの反映

}

//=============================================================================
// カメラの向きを取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return g_rotcamera;
}
//=============================================================================
// カメラの座標を渡す
//=============================================================================
D3DXVECTOR3 GetCameraP(void)
{
	return g_poscameraP;
}
//=============================================================================
// カメラを動かす
//=============================================================================
void PoseCameUpdate(void)
{
			if(GetKeyboardPress(DIK_W))
	{

		g_poscameraP.x+=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z+=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}

	if(GetKeyboardPress(DIK_S))
	{
		g_poscameraP.x-=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z-=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}
	if(GetKeyboardPress(DIK_D))
	{
		g_poscameraP.x+=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z-=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}
	if(GetKeyboardPress(DIK_A))
	{
		g_poscameraP.x-=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z+=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}

		if(GetKeyboardPress(DIK_Z))
	{
		g_rotcamera.y-=C_ROTMOVE;
		g_poscameraP.x=g_poscameraR.x-(g_veccamera*sinf(g_rotcamera.y));
		g_poscameraP.z=g_poscameraR.z-(g_veccamera*cosf(g_rotcamera.y));

	}

	if(GetKeyboardPress(DIK_X))
	{
		g_rotcamera.y+=C_ROTMOVE;
		g_poscameraP.x=g_poscameraR.x-(g_veccamera*sinf(g_rotcamera.y));
		g_poscameraP.z=g_poscameraR.z-(g_veccamera*cosf(g_rotcamera.y));
	}


	if(GetKeyboardPress(DIK_Q))
	{
		g_rotcamera.y-=C_ROTMOVE;
		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}
	if(GetKeyboardPress(DIK_E))
	{
		g_rotcamera.y+=C_ROTMOVE;
		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;
	}
	
	if(GetKeyboardPress(DIK_Y))
	{
		g_poscameraP.y+=C_MOVEY;
		g_poscameraR.y+=C_MOVEY;
	}	

	if(GetKeyboardPress(DIK_B))
	{ 
		g_poscameraP.y-=C_MOVEY;
		g_poscameraR.y-=C_MOVEY;
	}
		if(GetKeyboardPress(DIK_U))
	{
		g_poscameraR.y += C_MOVEY;
	}	

	if(GetKeyboardPress(DIK_N))
	{ 
		g_poscameraR.y -= C_MOVEY;
	}

}
//=============================================================================
// カメラの注視点座標を渡す
//=============================================================================
D3DXVECTOR3 GetCameraR(void)
{
	return g_poscameraR;
}
D3DXMATRIX GetMatrixView()
{
	return g_mtxview;
}

//End Of File