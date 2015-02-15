//=============================================================================
//
// 結果背景 [resultbg.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "resultbg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_BULLET (" ")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexResultBG(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureResultBG = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffResultBG = NULL;	// 頂点バッファインターフェースへのポインタ
int g_nResultFlag = 0;
int g_nResultScore;//リザルト用のスコアの値保持
int g_nNumEnemyScore;//敵を何体倒したか
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitResultBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_2D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffResultBG,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexResultBG();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureResultBG);

		g_nResultScore += g_nNumEnemyScore * 25;
	FILE *fp;
	fp=fopen(FILE_R,"ab");
	fwrite(&g_nResultScore,sizeof(int),1,fp);
	fclose(fp);

	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitResultBG(void)
{
	if(g_pD3DVtxBuffResultBG!=NULL)
	{
		g_pD3DVtxBuffResultBG->Release();//初期化
		g_pD3DVtxBuffResultBG=NULL;
	}
		if(g_pD3DTextureResultBG!=NULL)//空でないなら
	{
		g_pD3DTextureResultBG->Release();
		g_pD3DTextureResultBG=NULL;//アドレスを空に
	}

}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateResultBG(void)
{
	if(g_nResultFlag == 0)
	{
	//結果背景更新
	PrintMessageProc("\n\n\n\n\n\n\n\n結果画面\n");
	PrintMessageProc("倒した敵の数: %d\n",g_nNumEnemyScore);
	PrintMessageProc("敵の数のスコア: %d\n",g_nNumEnemyScore * 25);
	PrintMessageProc("スコア: %d\n",g_nResultScore);
	PrintMessageProc("エンターでタイトルへ");
	}else{
	//結果背景更新
	PrintMessageProc("ゲームオーバー\n");
	PrintMessageProc("エンターでタイトルへ");
	}
}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawResultBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポリゴンの描画

	pDevice->SetStreamSource(0,g_pD3DVtxBuffResultBG,0,sizeof(VERTEX_2D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_2D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureResultBG);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									4);//個数
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexResultBG(void)
{
	VERTEX_2D *pvtx;
	g_pD3DVtxBuffResultBG->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	
	//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,0,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH,0,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,SCREEN_HEIGHT,0.0f);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f);//頂点の座標格納
	if(g_nResultFlag == 0)
	{
	pvtx[0].diffuse = D3DCOLOR_RGBA(55,55,255,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(55,55,255,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(55,55,255,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(55,55,255,255); //色設定
	}else{
	pvtx[0].diffuse = D3DCOLOR_RGBA(255,25,25,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,25,25,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,25,25,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,25,25,255); //色設定
	}
	pvtx[0].rhw = 1.0f;
	pvtx[1].rhw = 1.0f;
	pvtx[2].rhw = 1.0f;
	pvtx[3].rhw = 1.0f;

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f/10,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f/10,1.0f);

g_pD3DVtxBuffResultBG->Unlock();//ロックの解除を忘れずに

}

void SetResultMode(int nMode)
{
	g_nResultFlag = nMode;
}

//*****************************************************************************
// スコアの値わたす
//*****************************************************************************
void SetScoreNum(int nNum)
{
	g_nResultScore = nNum;
}
//*****************************************************************************
// 敵の数わたす
//*****************************************************************************
void SetEnemyNum(int nNum)
{
	g_nNumEnemyScore = nNum;
}

//End Of File