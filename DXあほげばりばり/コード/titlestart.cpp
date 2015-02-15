//=============================================================================
//
// タイトルボタン [titlestart.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "titlestart.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_TITLELSTART ("data/TEXTURE/start.png")
#define START_ROT_SPEED (0.1f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexTitleStart(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitleStart = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleStart = NULL;	// 頂点バッファインターフェースへのポインタ
int g_nStartButtonCnt;
D3DXVECTOR3 g_fStartSpeed;
D3DXVECTOR3 g_fStartPos;
float g_fStartRot;
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitTitleStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_fStartRot = 0;
	g_fStartSpeed = D3DXVECTOR3(0,0,0);
	g_fStartPos = D3DXVECTOR3(TITLESTART_X,TITLESTART_Y,0);
	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffTitleStart,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexTitleStart();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_TITLELSTART,
								&g_pD3DTextureTitleStart);
		g_nStartButtonCnt = 0;
	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitTitleStart(void)
{
	if(g_pD3DVtxBuffTitleStart!=NULL)
	{
		g_pD3DVtxBuffTitleStart->Release();//初期化
		g_pD3DVtxBuffTitleStart=NULL;
	}
		if(g_pD3DTextureTitleStart!=NULL)//空でないなら
	{
		g_pD3DTextureTitleStart->Release();
		g_pD3DTextureTitleStart=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateTitleStart(void)
{
	g_nStartButtonCnt++;
	g_fStartRot += START_ROT_SPEED;
	if(g_fStartRot>D3DX_PI)
	{
		g_fStartRot = -D3DX_PI;
	}
	g_fStartPos.x += sinf(g_fStartRot);
	g_fStartPos.y += cosf(g_fStartRot);
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffTitleStart->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	
	//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y + TITLESTART_H,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y+ TITLESTART_H,0.0f);//頂点の座標格納
	g_pD3DVtxBuffTitleStart->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawTitleStart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポリゴンの描画

	pDevice->SetStreamSource(0,g_pD3DVtxBuffTitleStart,0,sizeof(VERTEX_2D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_2D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureTitleStart);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexTitleStart(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffTitleStart->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	
	//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(g_fStartPos.x,g_fStartPos.y + TITLESTART_H,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(g_fStartPos.x + TITLESTART_W,g_fStartPos.y+ TITLESTART_H,0.0f);//頂点の座標格納
	for(int loop=0;loop<4;loop++)
	{
	pvtx[loop].diffuse = D3DCOLOR_RGBA(255,255,20,255); //色設定
	
	pvtx[loop].rhw = 1.0f;
	}
	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,		0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1,1.0f);

g_pD3DVtxBuffTitleStart->Unlock();//ロックの解除を忘れずに

}

//End Of File