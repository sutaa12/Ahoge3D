//=============================================================================
//
// 地面処理 [wall.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SIZE_X (400)
#define SIZE_Z (500)
#define TEX_FIELD ("data/TEXTURE/wall.dds")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureWall = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxviewW;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionW;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldW;		//ワールドマトリックス


D3DXVECTOR3 g_posWall;		//地面の位置
D3DXVECTOR3 g_rotWall;		//地面の向き
D3DXVECTOR3 g_sclWall;		//地面の大きさ
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールド初期化
	g_posWall = D3DXVECTOR3(0.0,0.0,0.0);
	g_rotWall = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclWall = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffWall,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexWall();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureWall);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
		if(g_pD3DVtxBuffWall!=NULL)
	{
		g_pD3DVtxBuffWall->Release();//初期化
		g_pD3DVtxBuffWall=NULL;
	}
		if(g_pD3DTextureWall!=NULL)//空でないなら
	{
		g_pD3DTextureWall->Release();
		g_pD3DTextureWall=NULL;//アドレスを空に
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	float rot_y=0;

	D3DXMatrixIdentity(&g_mtxworldW);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,0.0f,0.0f,FIELDSIZEMAX);//XYZ
	D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldW);

	//ポリゴンの描画
	pDevice->SetStreamSource(0,g_pD3DVtxBuffWall,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureWall);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffWall->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	for(int nLoop=0;nLoop<3;nLoop++)
	{
			g_pD3DVtxBuffWall->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

		rot_y+=D3DX_PI/2;
		D3DXMatrixRotationYawPitchRoll(&mtxRot,rot_y,0.0f,0.0f);//YXZ
		D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxRot);//回転の反映
						for(int nLoop2=0;nLoop2<4;nLoop2++)
			{
				pvtx[nLoop2].nor = D3DXVECTOR3(0.0f,0,-1.0f);//法線の座標格納
			}
		if(nLoop==2)
		{
		D3DXMatrixTranslation(&mtxTranslate,FIELDSIZEMAX,0,0);//XYZ
		}else
		if(nLoop==1)
		{
		D3DXMatrixTranslation(&mtxTranslate,FIELDSIZEMAX,0,FIELDSIZEMAX);//XYZ
		}else
		if(nLoop==0)
		{
				for(int nLoop2=0;nLoop2<4;nLoop2++)
			{
				pvtx[nLoop2].nor = D3DXVECTOR3(0.0f,0,-1.0f);//法線の座標格納
			}
			D3DXMatrixTranslation(&mtxTranslate,0,0,FIELDSIZEMAX);//XYZ
		}
		D3DXMatrixMultiply(&g_mtxworldW,&g_mtxworldW,&mtxTranslate);//ワールドの反映
		pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldW);



	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数
	}


	g_pD3DVtxBuffWall->Unlock();//解除

}

//=============================================================================
// 頂点の初期化処理
//=============================================================================
void InitVertexWall(void)
{

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffWall->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,SIZE_Z,0);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(FIELDSIZEMAX,SIZE_Z,0);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(FIELDSIZEMAX,0.0f,0);//頂点の座標格納

	pvtx[0].nor = D3DXVECTOR3(0.0f,0,1.0f);//法線の座標格納
	pvtx[1].nor = D3DXVECTOR3(0.0f,0,1.0f);//法線の座標格納
	pvtx[2].nor = D3DXVECTOR3(0.0f,0,1.0f);//法線の座標格納
	pvtx[3].nor = D3DXVECTOR3(0.0f,0,1.0f);//法線の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pD3DVtxBuffWall->Unlock();//ロックの解除を忘れずに

}

//End Of File