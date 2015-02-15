//=============================================================================
//
// 弾の処理 [explo.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "explo.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_BULLET ("data/TEXTURE/shadow000.jpg")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
D3DXMATRIX GetMatrixView(void);
void InitVertexExplo(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureExplo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExplo = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxviewExplo;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionExplo;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldExplo;		//ワールドマトリックス


D3DXVECTOR3 g_posExplo;		//地面の位置
D3DXVECTOR3 g_rotExplo;		//地面の向き
D3DXVECTOR3 g_sclExplo;		//地面の大きさ

EXPLODATA g_ExploData[E_MAX];//弾データの配列
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitExplo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールド初期化
	g_posExplo = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotExplo = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclExplo = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffExplo,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexExplo();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureExplo);
	//配列初期化
		for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{

		//フラグと座標セット
		g_ExploData[nLoop].nFlag = 0;
		g_ExploData[nLoop].nPosx = 0;
		g_ExploData[nLoop].nPosy = 0;
		g_ExploData[nLoop].nPosz = 0;
		g_ExploData[nLoop].fSizex = 1.0f;
		g_ExploData[nLoop].fSizey = 1.0f;
		g_ExploData[nLoop].fSizez = 1.0f;
		g_ExploData[nLoop].fAlpha = 1.0f;
		g_ExploData[nLoop].nRot = 0;

	}
	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitExplo(void)
{
	if(g_pD3DVtxBuffExplo!=NULL)
	{
		g_pD3DVtxBuffExplo->Release();//初期化
		g_pD3DVtxBuffExplo=NULL;
	}
		if(g_pD3DTextureExplo!=NULL)//空でないなら
	{
		g_pD3DTextureExplo->Release();
		g_pD3DTextureExplo=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateExplo(void)
{
	for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{
		if(g_ExploData[nLoop].nFlag == 1)
		{
			g_ExploData[nLoop].fSizex += E_SIZESPEED;//サイズ変更
			g_ExploData[nLoop].fSizey += E_SIZESPEED;//サイズ変更
			g_ExploData[nLoop].fSizez += E_SIZESPEED;//サイズ変更
			g_ExploData[nLoop].fAlpha -= E_ALPHASPEED;//サイズ変更
			//αが0になったら0にする
			if(g_ExploData[nLoop].fAlpha < 0)
			{
				g_ExploData[nLoop].nFlag = 0;

			}
		}
	}
}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawExplo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;

	
		//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	mtxView = GetMatrixView();//カメラのビューをとってくる

	D3DXMatrixIdentity(&g_mtxworldExplo);//Identeyをだす
	D3DXMatrixInverse(&g_mtxworldExplo,NULL,&mtxView);//逆行列
	g_mtxworldExplo._41 = 0.0f;
	g_mtxworldExplo._42 = 0.0f;
	g_mtxworldExplo._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,g_posExplo.x,g_posExplo.y,g_posExplo.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldExplo);

	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);
	for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{
		if(g_ExploData[nLoop].nFlag == 1)//フラグが立っていないと
		{
				VERTEX_3D *pvtx;
				g_pD3DVtxBuffExplo->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				pvtx[0].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //色設定
				pvtx[1].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //色設定
				pvtx[2].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //色設定
				pvtx[3].diffuse = D3DXCOLOR(1.0f,0.8f,0.1f,g_ExploData[nLoop].fAlpha); //色設定

				g_pD3DVtxBuffExplo->Unlock();//ロックの解除を忘れずに


			D3DXMatrixIdentity(&g_mtxworldExplo);//ボールのIdenteyをだす
			D3DXMatrixInverse(&g_mtxworldExplo,NULL,&mtxView);//逆行列
			g_mtxworldExplo._41 = 0.0f;
			g_mtxworldExplo._42 = 0.0f;
			g_mtxworldExplo._43 = 0.0f;

			g_posExplo = D3DXVECTOR3(g_ExploData[nLoop].fSizex,g_ExploData[nLoop].fSizey,g_ExploData[nLoop].fSizez);

			D3DXMatrixScaling(&mtxScl,g_posExplo.x,g_posExplo.y,g_posExplo.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxScl);//スケールの反映

			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
			D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxRot);//回転の反映

			g_posExplo = D3DXVECTOR3(	g_ExploData[nLoop].nPosx,
									g_ExploData[nLoop].nPosy,
									g_ExploData[nLoop].nPosz);

			D3DXMatrixTranslation(&mtxTranslate,g_posExplo.x,g_posExplo.y,g_posExplo.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldExplo,&g_mtxworldExplo,&mtxTranslate);//ワールドの反映

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldExplo);

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldExplo);
			//ポリゴンの描画

			pDevice->SetStreamSource(0,g_pD3DVtxBuffExplo,0,sizeof(VERTEX_3D));//ポリゴンセット

			pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

			pDevice->SetTexture(0,g_pD3DTextureExplo);//テクスチャセット
			for(int nCnt = 0; nCnt < 10; nCnt++)
			{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
											0,
											2);//個数
			}
		}

	}

	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexExplo(void)
{
	VERTEX_3D *pvtx;
	g_pD3DVtxBuffExplo->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(-E_SIZEWH / 2,E_SIZEWH / 2,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(E_SIZEWH / 2,E_SIZEWH / 2,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(-E_SIZEWH / 2,-E_SIZEWH / 2,0);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(E_SIZEWH / 2,-E_SIZEWH / 2,0);//頂点の座標格納

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

g_pD3DVtxBuffExplo->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// フラグセット
//*****************************************************************************
void SetExplo(float x,float y,float z,float rot)
{
	for(int nLoop = 0;nLoop < E_MAX;nLoop++)
	{
		if(g_ExploData[nLoop].nFlag != 1)//フラグが立っていないと
		{
			//フラグと座標セット
			g_ExploData[nLoop].nFlag = 1;
			g_ExploData[nLoop].nPosx = x;
			g_ExploData[nLoop].nPosy = y;
			g_ExploData[nLoop].nPosz = z;
			g_ExploData[nLoop].fSizex = 1.0f;
			g_ExploData[nLoop].fSizey = 1.0f;
			g_ExploData[nLoop].fSizez = 1.0f;
			g_ExploData[nLoop].fAlpha = 1.0f;
			g_ExploData[nLoop].nRot = rot;
			g_ExploData[nLoop].nCnt = 0;
			return;
		}
	}
}

D3DXVECTOR3 GetExploPos(int nNum)
{
	D3DXVECTOR3 vector = D3DXVECTOR3(g_ExploData[nNum].nPosx,g_ExploData[nNum].nPosy,g_ExploData[nNum].nPosz);
	return vector;
}

//End Of FIle