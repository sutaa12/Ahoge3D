//=============================================================================
//
// メッシュドームの処理 [meshdoom.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "meshdoom.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_SKY ("data/TEXTURE/sky.jpg")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertexDoom(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureDoom;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffDoom;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBuffDoom;//インデックスバッファ

D3DXMATRIX g_mtxWorldDoom;					// ワールドマトリックス
D3DXVECTOR3 g_posDoom;						// 位置
D3DXVECTOR3 g_rotDoom;						// 向き

int g_nNumDoomBlockX, g_nNumDoomBlockZ;				// ブロック数
int g_nNumDoomVertex;							// 総頂点数
int g_nNumDoomPolygon;							// 総ポリゴン数
float g_fSizeDoomBlockX, g_fSizeDoomBlockZ;			// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshDoom(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//フィールド初期化
	g_posDoom = D3DXVECTOR3(100.0,0.0,100.0);
	g_rotDoom = D3DXVECTOR3(0.0,0.0,0.0);
	g_nNumDoomBlockX = nNumBlockX;
	g_nNumDoomBlockZ = nNumBlockZ;
	g_fSizeDoomBlockX = fSizeBlockX;
	g_fSizeDoomBlockZ = fSizeBlockZ;
	g_nNumDoomVertex = (nNumBlockX+2)*(nNumBlockZ*2)-2;//総数検出
	g_nNumDoomPolygon = (nNumBlockX+1)*(nNumBlockZ+1);//ポリゴン数
	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*g_nNumDoomPolygon,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffDoom,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}

	//インデックス頂点情報の設定
	if(FAILED(pDevice->CreateIndexBuffer(	//頂点バッファ作成
				sizeof(WORD)*g_nNumDoomVertex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_pD3DIndexBuffDoom,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}

	InitVertexDoom();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_SKY,
								&g_pD3DTextureDoom);
	return S_OK;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshDoom(void)
{
	if(g_pD3DVtxBuffDoom!=NULL)
	{
		g_pD3DVtxBuffDoom->Release();//初期化
		g_pD3DVtxBuffDoom=NULL;
	}
	if(g_pD3DTextureDoom!=NULL)//空でないなら
	{
		g_pD3DTextureDoom->Release();
		g_pD3DTextureDoom=NULL;//アドレスを空に
	}
	if(&g_pD3DIndexBuffDoom!=NULL)
	{
		g_pD3DIndexBuffDoom->Release();
		g_pD3DIndexBuffDoom=NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshDoom(void)
{
	g_rotDoom.y+=0.01f;
	if(g_rotDoom.y>D3DX_PI)
	{
		g_rotDoom.y = -D3DX_PI;
	}
}

//=============================================================================
// 描画初理
//=============================================================================
void DrawMeshDoom(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMatrixIdentity(&g_mtxWorldDoom);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldDoom,&g_mtxWorldDoom,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,g_rotDoom.y,g_rotDoom.x,g_rotDoom.z);//YXZ
	D3DXMatrixMultiply(&g_mtxWorldDoom,&g_mtxWorldDoom,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,0,0,0);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldDoom,&g_mtxWorldDoom,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxWorldDoom);


	//ポリゴンの描画
	pDevice->SetStreamSource(0,g_pD3DVtxBuffDoom,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetIndices(g_pD3DIndexBuffDoom);

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureDoom);//テクスチャセット

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									0,
									g_nNumDoomVertex,
									0,
									(g_nNumDoomBlockX*g_nNumDoomBlockZ)*2+((g_nNumDoomBlockZ-1)*2));//個数


}

//=============================================================================
// 頂点の初期化処理
//=============================================================================
void InitVertexDoom(void)
{

	VERTEX_3D *pvtx;
	
	g_pD3DVtxBuffDoom->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	int nNum=0;
	for(int nLoopZ=0;nLoopZ<=g_nNumDoomBlockZ;nLoopZ++)//Zブロック数+1まで
	{
		for(int nLoopX=0;nLoopX<=g_nNumDoomBlockX;nLoopX++)//Xブロック数+1まで
		{
			pvtx[nNum].vtx = D3DXVECTOR3(sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ))	* (cosf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) * ( g_fSizeDoomBlockX / (g_nNumDoomBlockX)),
										cosf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * ( g_fSizeDoomBlockZ / (g_nNumDoomBlockZ)),
										sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * (sinf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) * ( g_fSizeDoomBlockX / (g_nNumDoomBlockX)) );//頂点の座標格納
			pvtx[nNum].nor = D3DXVECTOR3(sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ))	* (cosf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)/1),
										cosf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)/1),
										sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * (sinf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) /1);//法線の座標格納
			pvtx[nNum].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
			pvtx[nNum].tex = D3DXVECTOR2((sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ))	* (cosf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)) / 1.f),
										sinf(D3DX_PI * 2 / (g_nNumDoomBlockZ) * (nLoopZ)) * (sinf((D3DX_PI * 2 / g_nNumDoomBlockX) * nLoopX)/1.f));
			nNum++;
		}
	}
	g_pD3DVtxBuffDoom->Unlock();//ロックの解除を忘れずに

	WORD *pIndex;
	
	g_pD3DIndexBuffDoom->Lock(0,0,(void**)&pIndex,0);
	int nIndexNum = g_nNumDoomBlockX+1;//インデックス頂点の位置
	int nIndexZcnt = 0;//上頂点の位置
	int nIndexReturn = 0;//折り返し回数

	for(int nBuff = 0 ; nBuff < g_nNumDoomVertex ; nBuff++)
	{
		pIndex[nBuff] = nIndexNum;//下の設定
		nBuff++;
		pIndex[nBuff] = nIndexZcnt;//上の設定
		nIndexNum++;
		nIndexZcnt++;
		if(nIndexNum-nIndexReturn*2%g_nNumDoomBlockX + (g_nNumDoomBlockX*nIndexReturn) == 0&&nBuff < g_nNumDoomVertex)//上の頂点が横サイズで割り切れれば
		{
			nBuff++;
			pIndex[nBuff] = nIndexNum;//下の設定
			nBuff++;
			pIndex[nBuff] = nIndexZcnt;//上の設定
		}
	}
 	g_pD3DIndexBuffDoom->Unlock();//ロック

}

//End Of File