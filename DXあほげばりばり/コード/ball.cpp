//=============================================================================
//
// ビルボード処理 [ball.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "ball.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SIZE_X (20)
#define SIZE_Z (20)
#define TEX_FIELD ("data/TEXTURE/bullet000.png")
#define MOVE_POS (5)

//*****************************************************************************
// プロトタイプ宣言
//*****え************************************************************************

void InitVertexBall(void);
D3DXMATRIX GetMatrixView(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBall = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBall = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxviewB;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionB;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldB;		//ワールドマトリックス


D3DXVECTOR3 g_posBall;		//地面の位置
D3DXVECTOR3 g_rotBall;		//地面の向き
D3DXVECTOR3 g_sclBall;		//地面の大きさ
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールド初期化
	g_posBall = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotBall = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclBall = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffBall,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexBall();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureBall);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBall(void)
{
		if(g_pD3DVtxBuffBall!=NULL)
	{
		g_pD3DVtxBuffBall->Release();//初期化
		g_pD3DVtxBuffBall=NULL;
	}
		if(g_pD3DTextureBall!=NULL)//空でないなら
	{
		g_pD3DTextureBall->Release();
		g_pD3DTextureBall=NULL;//アドレスを空に
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBall(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;
	mtxView = GetMatrixView();//カメラのビューをとってくる
	D3DXMatrixIdentity(&g_mtxworldB);//ボールのIdenteyをだす
	D3DXMatrixInverse(&g_mtxworldB,NULL,&mtxView);//逆行列
	g_mtxworldB._41 = 0.0f;
	g_mtxworldB._42 = 0.0f;
	g_mtxworldB._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldB,&g_mtxworldB,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldB,&g_mtxworldB,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,g_posBall.x,g_posBall.y,g_posBall.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldB,&g_mtxworldB,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldB);




	//ポリゴンの描画

	pDevice->SetStreamSource(0,g_pD3DVtxBuffBall,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureBall);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数


}

//=============================================================================
// 頂点の初期化処理
//=============================================================================
void InitVertexBall(void)
{

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffBall->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,SIZE_Z,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(SIZE_X,SIZE_Z,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(SIZE_X,0.0f,0);//頂点の座標格納

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

g_pD3DVtxBuffBall->Unlock();//ロックの解除を忘れずに

}

D3DXVECTOR3 GetEnemyField()
{
	return g_posBall;
}

//End Of FIle