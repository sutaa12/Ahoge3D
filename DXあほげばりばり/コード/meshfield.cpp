//=============================================================================
//
// メッシュ地面の処理 [meshfield.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_FIELD ("data/TEXTURE/field1.dds")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitVertex(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	g_pD3DIndexBuffField;//インデックスバッファ

D3DXMATRIX g_mtxWorldField;					// ワールドマトリックス
D3DXVECTOR3 g_posField;						// 位置
D3DXVECTOR3 g_rotField;						// 向き

int g_nNumBlockX, g_nNumBlockZ;				// ブロック数
int g_nNumVertex;							// 総頂点数
int g_nNumPolygon;							// 総ポリゴン数
float g_fSizeBlockX, g_fSizeBlockZ;			// ブロックサイズ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//フィールド初期化
	g_posField = D3DXVECTOR3(0.0,0.0,0.0);
	g_rotField = D3DXVECTOR3(0.0,0.0,0.0);
	g_nNumBlockX = nNumBlockX;
	g_nNumBlockZ = nNumBlockZ;
	g_fSizeBlockX = fSizeBlockX;
	g_fSizeBlockZ = fSizeBlockZ;
	g_nNumVertex = (nNumBlockX+2)*(nNumBlockZ*2)-2;//総数検出
	g_nNumPolygon = (nNumBlockX+1)*(nNumBlockZ+1);//ポリゴン数
	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*g_nNumPolygon,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffField,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}

	//インデックス頂点情報の設定
	if(FAILED(pDevice->CreateIndexBuffer(	//頂点バッファ作成
				sizeof(WORD)*g_nNumVertex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_pD3DIndexBuffField,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}

	InitVertex();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureField);
	return S_OK;


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	if(g_pD3DVtxBuffField!=NULL)
	{
		g_pD3DVtxBuffField->Release();//初期化
		g_pD3DVtxBuffField=NULL;
	}
	if(g_pD3DTextureField!=NULL)//空でないなら
	{
		g_pD3DTextureField->Release();
		g_pD3DTextureField=NULL;//アドレスを空に
	}
	if(&g_pD3DIndexBuffField!=NULL)
	{
		g_pD3DIndexBuffField->Release();
		g_pD3DIndexBuffField=NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画初理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMatrixIdentity(&g_mtxWorldField);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldField,&g_mtxWorldField,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxWorldField,&g_mtxWorldField,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,FIELDSIZEMAX/2,0.0f,FIELDSIZEMAX/2);//XYZ
	D3DXMatrixMultiply(&g_mtxWorldField,&g_mtxWorldField,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxWorldField);


	//ポリゴンの描画
	pDevice->SetStreamSource(0,g_pD3DVtxBuffField,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetIndices(g_pD3DIndexBuffField);

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureField);//テクスチャセット

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									0,
									g_nNumVertex,
									0,
									(g_nNumBlockX*g_nNumBlockZ)*2+((g_nNumBlockZ-1)*2));//個数


}

//=============================================================================
// 頂点の初期化処理
//=============================================================================
void InitVertex(void)
{

	VERTEX_3D *pvtx;
	
	g_pD3DVtxBuffField->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
	int nNum=0;
	for(int nLoopZ=0;nLoopZ<=g_nNumBlockZ;nLoopZ++)//Zブロック数+1まで
	{
		for(int nLoopX=0;nLoopX<=g_nNumBlockX;nLoopX++)//Xブロック数+1まで
		{
			pvtx[nNum].vtx = D3DXVECTOR3((	g_fSizeBlockX/(g_nNumBlockX))*nLoopX - (g_fSizeBlockX / 2),
										0.0f,
										(g_fSizeBlockZ/(g_nNumBlockZ))*(g_nNumBlockZ-nLoopZ) - (g_fSizeBlockZ / 2));//頂点の座標格納
			pvtx[nNum].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//法線の座標格納
			pvtx[nNum].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
			pvtx[nNum].tex = D3DXVECTOR2((float)nLoopX,(float)nLoopZ);
			nNum++;
		}
	}
	g_pD3DVtxBuffField->Unlock();//ロックの解除を忘れずに

	WORD *pIndex;
	
	g_pD3DIndexBuffField->Lock(0,0,(void**)&pIndex,0);

	int nIndexNum = g_nNumBlockX+1;//インデックス頂点の位置
	int nIndexZcnt = 0;//上頂点の位置
	int nIndexReturn = 0;//折り返し回数

	for(int nBuff = 0 ; nBuff < g_nNumVertex ; nBuff++)
	{
		pIndex[nBuff] = nIndexNum;//下の設定
		nBuff++;
		pIndex[nBuff] = nIndexZcnt;//上の設定
		nIndexNum++;
		nIndexZcnt++;
		if(nIndexNum-nIndexReturn*2%g_nNumBlockX + (g_nNumBlockX*nIndexReturn) == 0&&nBuff < g_nNumVertex)//上の頂点が横サイズで割り切れれば
		{
			nBuff++;
			pIndex[nBuff] = nIndexNum;//下の設定
			nBuff++;
			pIndex[nBuff] = nIndexZcnt;//上の設定
		}
	}
 	g_pD3DIndexBuffField->Unlock();//ロック

}

//End Of File