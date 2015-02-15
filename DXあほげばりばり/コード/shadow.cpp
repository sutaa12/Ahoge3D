//=============================================================================
//
// 地面処理 [field.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "shadow.h"
#include "camera.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SIZE_X (20)
#define SIZE_Z (20)
#define TEX_FIELD ("data/TEXTURE/shadow000.jpg")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void InitVertexShadow(void);
D3DXVECTOR3 GetEnemyPosition(int nNum);
SHADOWDATA g_posshadow[SHADOW_MAX];

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureShadow = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffShadow = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxviewF;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionF;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldF;		//ワールドマトリックス


D3DXVECTOR3 g_posShadow;		//地面の位置
D3DXVECTOR3 g_rotShadow;		//地面の向き
D3DXVECTOR3 g_sclShadow;		//地面の大きさ
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールド初期化
	g_posShadow = D3DXVECTOR3(0.0,0.0,0.0);
	g_rotShadow = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclShadow = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffShadow,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexShadow();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_FIELD,
								&g_pD3DTextureShadow);

	for(int nLoop = 0;nLoop < SHADOW_MAX;nLoop++)
	{
		g_posshadow[nLoop].nPosx = 0;
		g_posshadow[nLoop].nPosz = 0;

	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
		if(g_pD3DVtxBuffShadow!=NULL)
	{
		g_pD3DVtxBuffShadow->Release();//初期化
		g_pD3DVtxBuffShadow=NULL;
	}
		if(g_pD3DTextureShadow!=NULL)//空でないなら
	{
		g_pD3DTextureShadow->Release();
		g_pD3DTextureShadow=NULL;//アドレスを空に
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
	for(int nLoop = 0;nLoop < SHADOW_MAX;nLoop++)
	{
		if(GetBulletFlag(nLoop) == 1)
		{
			D3DXVECTOR3 vector = GetBulletPos(nLoop);
			g_posshadow[nLoop].nPosx = vector.x;
			g_posshadow[nLoop].nPosz = vector.z;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;
	
	//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	//プレイヤーの影描画
	D3DXMatrixIdentity(&g_mtxworldF);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,GetPlayerPosition().x - 8,0.01f,GetPlayerPosition().z - 8);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldF);


	//ポリゴンの描画


	pDevice->SetStreamSource(0,g_pD3DVtxBuffShadow,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureShadow);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数

	//敵の影描画
	for(int nLoop = 0;nLoop < ENEMY_MAX;nLoop++)
	{
		if(GetEnemyLife(nLoop) == 1)
		{
			D3DXMatrixIdentity(&g_mtxworldF);
			D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
			D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxScl);//スケールの反映

			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
			D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxRot);//回転の反映

			D3DXMatrixTranslation(&mtxTranslate,GetEnemyPosition(nLoop).x,0.01f,GetEnemyPosition(nLoop).z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxTranslate);//ワールドの反映

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldF);


			//ポリゴンの描画


			pDevice->SetStreamSource(0,g_pD3DVtxBuffShadow,0,sizeof(VERTEX_3D));//ポリゴンセット

			pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

			pDevice->SetTexture(0,g_pD3DTextureShadow);//テクスチャセット

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
											0,
											2);//個数
		}

	}

	for(int nLoop = 0;nLoop < SHADOW_MAX;nLoop++)
	{
		if(GetBulletFlag(nLoop) == 1)
		{
			D3DXVECTOR3 vector = GetBulletPos(nLoop);
				D3DXMatrixIdentity(&g_mtxworldF);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxRot);//回転の反映
	D3DXMatrixTranslation(&mtxTranslate,g_posshadow[nLoop].nPosx,0.01f,g_posshadow[nLoop].nPosz);//XYZ
	D3DXMatrixMultiply(&g_mtxworldF,&g_mtxworldF,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldF);


	//ポリゴンの描画

	//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);

	pDevice->SetStreamSource(0,g_pD3DVtxBuffShadow,0,sizeof(VERTEX_3D));//ポリゴンセット

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	pDevice->SetTexture(0,g_pD3DTextureShadow);//テクスチャセット

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
									0,
									2);//個数
		}
	}

		//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);


}

//=============================================================================
// 頂点の初期化処理
//=============================================================================
void InitVertexShadow(void)
{

	VERTEX_3D *pvtx;
	g_pD3DVtxBuffShadow->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,0.0f,SIZE_Z);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(SIZE_X,0.0f,SIZE_Z);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(SIZE_X,0.0f,0);//頂点の座標格納

	pvtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//法線の座標格納
	pvtx[1].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//法線の座標格納
	pvtx[2].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//法線の座標格納
	pvtx[3].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);//法線の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,150); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,150); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,150); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,150); //色設定

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	g_pD3DVtxBuffShadow->Unlock();//ロックの解除を忘れずに

}

//End Of File