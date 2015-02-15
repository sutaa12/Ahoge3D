//=============================================================================
//
// メッシュ地面の処理 [meshroll.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "meshroll.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FIELD ("data/TEXTURE/field001.jpg")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexRoll(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureRoll;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffRoll;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBuffRoll;//インデックスバッファ

D3DXMATRIX g_mtxWorldRoll;					// ワールドマトリックス
D3DXVECTOR3 g_posRoll;						// 位置
D3DXVECTOR3 g_rotRoll;						// 向き

int g_nNumRollBlockX, g_nNumRollBlockZ;				// ブロック数
int g_nNumRollVertex;							// 総頂点数
int g_nNumRollPolygon;							// 総ポリゴン数
float g_fSizeRollBlockX, g_fSizeRollBlockZ;			// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshRoll(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//フィールド初期化
	g_posRoll = D3DXVECTOR3(100.0,0.0,100.0);
	g_rotRoll = D3DXVECTOR3(0.0,0.0,0.0);
	g_nNumRollBlockX = nNumBlockX;
	g_nNumRollBlockZ = nNumBlockZ;
	g_fSizeRollBlockX = fSizeBlockX;
	g_fSizeRollBlockZ = fSizeBlockZ;
	g_nNumRollVertex = (nNumBlockX+2)*(nNumBlockZ*2)-2;//総数検出
	g_nNumRollPolygon = (nNumBlockX+1)*(nNumBlockZ+1);//ポリゴン数
	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*g_nNumRollPolygon,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffRoll,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}

	//インデックス頂点情報の設定
	if(FAILED(pDevice->CreateIndexBuffer(	//頂点バッファ作成
				sizeof(WORD)*g_nNumRollVertex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_pD3DIndexBuffRoll,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}

	InitVertexRoll();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureRoll);
	return S_OK;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshRoll(void)
{
	if(g_pD3DVtxBuffRoll!=NULL)
	{
		g_pD3DVtxBuffRoll->Release();//初期化
		g_pD3DVtxBuffRoll=NULL;
	}
	if(g_pD3DTextureRoll!=NULL)//空でないなら
	{
		g_pD3DTextureRoll->Release();
		g_pD3DTextureRoll=NULL;//アドレスを空に
	}
	if(&g_pD3DIndexBuffRoll!=NULL)
	{
		g_pD3DIndexBuffRoll->Release();
		g_pD3DIndexBuffRoll=NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshRoll(void)
{

}

//=============================================================================
// 描画初理
//=============================================================================
void DrawMeshRoll(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMatrixIdentity(&g_mtxWorldRoll);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldRoll,&g_mtxWorldRoll,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxWorldRoll,&g_mtxWorldRoll,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,-400.0f,0.0f,150.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldRoll,&g_mtxWorldRoll,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxWorldRoll);


	//ポリゴンの描画
	pDevice->SetStreamSource(0,g_pD3DVtxBuffRoll,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetIndices(g_pD3DIndexBuffRoll);

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureRoll);//テクスチャセット

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									0,
									g_nNumRollVertex,
									0,
									(g_nNumRollBlockX*g_nNumRollBlockZ)*2+((g_nNumRollBlockZ-1)*4));//個数


}

//=============================================================================
// 頂点の初期化処理
//=============================================================================
void InitVertexRoll(void)
{

	VERTEX_3D *pvtx;
	
	g_pD3DVtxBuffRoll->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	int nNum=0;
	for(int nLoopZ=0;nLoopZ<=g_nNumRollBlockZ;nLoopZ++)//Zブロック数+1まで
	{
		for(int nLoopX=0;nLoopX<=g_nNumRollBlockX;nLoopX++)//Xブロック数+1まで
		{
			pvtx[nNum].vtx = D3DXVECTOR3(	(cosf((D3DX_PI * 2 / g_nNumRollBlockX) * nLoopX)) * ( g_fSizeRollBlockX/(g_nNumRollBlockX)),
										(g_fSizeRollBlockZ/(g_nNumRollBlockZ)) * (g_nNumRollBlockZ-nLoopZ),
										(sinf((D3DX_PI * 2 / g_nNumRollBlockX) * nLoopX)) * ( g_fSizeRollBlockX/(g_nNumRollBlockX)) );//頂点の座標格納
			pvtx[nNum].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
			pvtx[nNum].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
			pvtx[nNum].tex = D3DXVECTOR2((float)nLoopX,(float)nLoopZ);
			nNum++;
		}
	}
	g_pD3DVtxBuffRoll->Unlock();//ロックの解除を忘れずに

	WORD *pIndex;
	
	g_pD3DIndexBuffRoll->Lock(0,0,(void**)&pIndex,0);

	int nIndexNum = g_nNumRollBlockX+1;//インデックス頂点の位置
	int nIndexZcnt = 0;//上頂点の位置
	int nIndexReturn = 0;//折り返し回数

	for(int nBuff = 0 ; nBuff < g_nNumRollVertex ; nBuff++)
	{
		pIndex[nBuff] = nIndexNum;//下の設定
		nBuff++;
		pIndex[nBuff] = nIndexZcnt;//上の設定
		nIndexNum++;
		nIndexZcnt++;
		if(nIndexNum-nIndexReturn*2%g_nNumRollBlockX + (g_nNumRollBlockX*nIndexReturn) == 0&&nBuff < g_nNumRollVertex)//上の頂点が横サイズで割り切れれば
		{
			nBuff++;
			pIndex[nBuff] = nIndexNum;//下の設定
			nBuff++;
			pIndex[nBuff] = nIndexZcnt;//上の設定
		}
	}
 	g_pD3DIndexBuffRoll->Unlock();//ロック

}

//End Of File