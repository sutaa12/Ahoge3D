//=============================================================================
//
// タイトル背景 [titlebg.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "titlelogo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_TITLELOGO ("data/TEXTURE/titlelogo.png")
#define LOGO_ROT_SPEED (0.05f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexTitleLogo(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ
int g_nTitleCnt;
int g_nLogoButtonCnt;
D3DXVECTOR3 g_fLogoSpeed;
D3DXVECTOR3 g_fLogoPos;
float g_fLogoRot;
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_fLogoRot = 0;
	g_fLogoSpeed = D3DXVECTOR3(0,0,0);
	g_fLogoPos = D3DXVECTOR3(TITLELOGO_X,TITLELOGO_Y,0);
	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffTitleLogo,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexTitleLogo();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_TITLELOGO,
								&g_pD3DTextureTitleLogo);
		g_nTitleCnt = 0;
	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitTitleLogo(void)
{
	if(g_pD3DVtxBuffTitleLogo!=NULL)
	{
		g_pD3DVtxBuffTitleLogo->Release();//初期化
		g_pD3DVtxBuffTitleLogo=NULL;
	}
		if(g_pD3DTextureTitleLogo!=NULL)//空でないなら
	{
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateTitleLogo(void)
{
	g_nTitleCnt++;
	VERTEX_2D *pvtx;
	g_nTitleCnt = g_nTitleCnt%90;
	g_pD3DVtxBuffTitleLogo->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	for (int nLoop = 0; nLoop < 4 ; nLoop++)
	{

		if(g_nTitleCnt >=60)
		{
		if(g_nTitleCnt%3 == 0)
		{
		pvtx[nLoop].diffuse = D3DCOLOR_RGBA(20,20,200,255);//色設定
		}else{
		pvtx[nLoop].diffuse = D3DCOLOR_RGBA(255,255,255,255);//色設定
		}

		}
	}
	g_nLogoButtonCnt++;
	g_fLogoRot += LOGO_ROT_SPEED;
	if(g_fLogoRot<D3DX_PI)
	{
		g_fLogoRot = D3DX_PI;
	}
	g_fLogoPos.x -= sinf(g_fLogoRot);
	g_fLogoPos.y -= cosf(g_fLogoRot);
	//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y + TITLELOGO_H,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y+ TITLELOGO_H,0.0f);//頂点の座標格納
	g_pD3DVtxBuffTitleLogo->Unlock();//ロックの解除を忘れずに

	g_pD3DVtxBuffTitleLogo->Unlock();//ロックの解除を忘れずに
}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポリゴンの描画

	pDevice->SetStreamSource(0,g_pD3DVtxBuffTitleLogo,0,sizeof(VERTEX_2D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_2D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureTitleLogo);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexTitleLogo(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffTitleLogo->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	
	//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(g_fLogoPos.x,g_fLogoPos.y + TITLELOGO_H,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(g_fLogoPos.x + TITLELOGO_W,g_fLogoPos.y+ TITLELOGO_H,0.0f);//頂点の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定

	pvtx[0].rhw = 1.0f;
	pvtx[1].rhw = 1.0f;
	pvtx[2].rhw = 1.0f;
	pvtx[3].rhw = 1.0f;

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,		0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1,1.0f);

g_pD3DVtxBuffTitleLogo->Unlock();//ロックの解除を忘れずに

}

//End Of File