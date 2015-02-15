//=============================================================================
//
// フェード処理 [feed.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "feed.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_BULLET (" ")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexFeed(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureFeed = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFeed = NULL;	// 頂点バッファインターフェースへのポインタ
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitFeed(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffFeed,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	//頂点初期化
	InitVertexFeed();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureFeed);
	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitFeed(void)
{
	if(g_pD3DVtxBuffFeed!=NULL)
	{
		g_pD3DVtxBuffFeed->Release();//初期化
		g_pD3DVtxBuffFeed=NULL;
	}
		if(g_pD3DTextureFeed!=NULL)//空でないなら
	{
		g_pD3DTextureFeed->Release();
		g_pD3DTextureFeed=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateFeed(float fFeed)
{
		VERTEX_2D *pvtx;
	g_pD3DVtxBuffFeed->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,(int)(fFeed * 255.f)); //色設定

	g_pD3DVtxBuffFeed->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawFeed(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポリゴンの描画

	pDevice->SetStreamSource(0,g_pD3DVtxBuffFeed,0,sizeof(VERTEX_2D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_2D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureFeed);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexFeed(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffFeed->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	
	//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,0,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);//頂点の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定

	pvtx[0].rhw = 1.0f;
	pvtx[1].rhw = 1.0f;
	pvtx[2].rhw = 1.0f;
	pvtx[3].rhw = 1.0f;

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f/10,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f/10,1.0f);

g_pD3DVtxBuffFeed->Unlock();//ロックの解除を忘れずに

}

//End Of File