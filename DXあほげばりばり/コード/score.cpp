//=============================================================================
//
// スコア処理 [score.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "score.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_BULLET ("data/TEXTURE/number000.png")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexScore(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffScore = NULL;	// 頂点バッファインターフェースへのポインタ
int g_nNumber = 0;
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_2D)*4*(UINT)SCORE_MAX,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffScore,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexScore();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureScore);
	g_nNumber = 0;

	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitScore(void)
{
	if(g_pD3DVtxBuffScore!=NULL)
	{
		g_pD3DVtxBuffScore->Release();//初期化
		g_pD3DVtxBuffScore=NULL;
	}
		if(g_pD3DTextureScore!=NULL)//空でないなら
	{
		g_pD3DTextureScore->Release();
		g_pD3DTextureScore=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateScore(void)
{

	VERTEX_2D *pvtx;
	int noldNumber = 0;
	int nNumberC = g_nNumber;
	
	g_pD3DVtxBuffScore->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
//スコアのセット	
for(int nCnt = 1;nCnt <= SCORE_MAX ; nCnt++)
	{
		noldNumber = (int)(nNumberC / pow(10,(double)(SCORE_MAX - nCnt)));
		pvtx[0].tex = D3DXVECTOR2((float)noldNumber / 10.0f,0.0f);
		pvtx[1].tex = D3DXVECTOR2((float)(noldNumber / 10.0f + 0.1),0.0f);
		pvtx[2].tex = D3DXVECTOR2((float)noldNumber / 10.0f,1.0f);
		pvtx[3].tex = D3DXVECTOR2((float)noldNumber / 10.0f+0.1f,1.0f);
		pvtx += 4;
		nNumberC -= (int)(noldNumber * pow(10,(double)(SCORE_MAX - nCnt)));
	}
	g_pD3DVtxBuffScore->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポリゴンの描画

	pDevice->SetStreamSource(0,g_pD3DVtxBuffScore,0,sizeof(VERTEX_2D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_2D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureScore);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									4*(UINT)SCORE_MAX);//個数
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexScore(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffScore->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	for(int nCnt = 0;nCnt < SCORE_MAX ; nCnt++)
	{
				//頂点情報の初期化

	pvtx[0].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt),SCORE_Y,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt)+SCORE_W,SCORE_Y,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt),SCORE_Y+SCORE_H,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(SCORE_X+(SCORE_W*nCnt)+SCORE_W,SCORE_Y+SCORE_H,0.0f);//頂点の座標格納

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
	pvtx += 4;
	}
g_pD3DVtxBuffScore->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// 加算
//*****************************************************************************
void AddScore(int nNum)
{
	g_nNumber += nNum;
	//最大桁数超えたら
	if(g_nNumber>pow((float)10,(float)SCORE_MAX))
	{
		g_nNumber = (int)pow((float)10,(float)SCORE_MAX) - 1;
	}else
	if(g_nNumber < 0)
	{
		g_nNumber = 0;
	}


}
//*****************************************************************************
// 現在値取得
//*****************************************************************************
int GetScore(void)
{
	return g_nNumber;
}
//End Of File