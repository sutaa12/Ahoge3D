//=============================================================================
//
// 弾の処理 [effect.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "effect.h"
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_EFFECT ("data/TEXTURE/effect.dds")

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
D3DXMATRIX GetMatrixView(void);
void InitVertexEffect(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxviewEffect;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionEffect;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldEffect;		//ワールドマトリックス


D3DXVECTOR3 g_posEffect;		//地面の位置
D3DXVECTOR3 g_rotEffect;		//地面の向き
D3DXVECTOR3 g_sclEffect;		//地面の大きさ

EFFECTDATA g_EffectData[EF_MAX];//弾データの配列
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールド初期化
	g_posEffect = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotEffect = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclEffect = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffEffect,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexEffect();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_EFFECT,
								&g_pD3DTextureEffect);
	//配列初期化
		for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{

		//フラグと座標セット
		g_EffectData[nLoop].nFlag = 0;
		g_EffectData[nLoop].nPosx = 0;
		g_EffectData[nLoop].nPosy = 0;
		g_EffectData[nLoop].nPosz = 0;
		//大きさセット
		g_EffectData[nLoop].fSizex = 1.0f;
		g_EffectData[nLoop].fSizey = 1.0f;
		g_EffectData[nLoop].fSizez = 1.0f;
		//色設定
		g_EffectData[nLoop].fAlpha = 1.0f;
		g_EffectData[nLoop].nRot = 0;
		g_EffectData[nLoop].dCol = 0;

	}
	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitEffect(void)
{
	if(g_pD3DVtxBuffEffect!=NULL)
	{
		g_pD3DVtxBuffEffect->Release();//初期化
		g_pD3DVtxBuffEffect=NULL;
	}
		if(g_pD3DTextureEffect!=NULL)//空でないなら
	{
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateEffect(void)
{
	for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{
		if(g_EffectData[nLoop].nFlag == 1)
		{

			int r,g,b;
			//色設定
			r = ((g_EffectData[nLoop].dCol >> 0) & 0xff);
			g = ((g_EffectData[nLoop].dCol >> 8) & 0xff);
			b = ((g_EffectData[nLoop].dCol >> 16) & 0xff);
			g_EffectData[nLoop].fAlpha -= EF_ALPHASPEED;//サイズ変更
			//色変更
			g_EffectData[nLoop].dCol = D3DCOLOR_RGBA(r,g,b,(int)(g_EffectData[nLoop].fAlpha * 255.f));

		//アニメーションカウントセット
			g_EffectData[nLoop].nCnt++;

			//αが0になったら0にする
			if(g_EffectData[nLoop].fAlpha < 0)
			{
				g_EffectData[nLoop].nFlag = 0;
			}
		}
	}
}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;

	
		//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);	


	mtxView = GetMatrixView();//カメラのビューをとってくる

	D3DXMatrixIdentity(&g_mtxworldEffect);//ボールのIdenteyをだす
	D3DXMatrixInverse(&g_mtxworldEffect,NULL,&mtxView);//逆行列
	g_mtxworldEffect._41 = 0.0f;
	g_mtxworldEffect._42 = 0.0f;
	g_mtxworldEffect._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,g_posEffect.x,g_posEffect.y,g_posEffect.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldEffect);

	for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{
		if(g_EffectData[nLoop].nFlag == 1)//フラグが立っていないと
		{
				VERTEX_3D *pvtx;
				g_pD3DVtxBuffEffect->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック
				
				for(int loop = 0;loop<4;loop++)
				{
				pvtx[loop].diffuse = g_EffectData[nLoop].dCol; //色設定
				}
				pvtx[0].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f,0.0f);
				pvtx[1].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f + 1.0f/4,0.0f);
				pvtx[2].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f,1.0f);
				pvtx[3].tex = D3DXVECTOR2((g_EffectData[nLoop].nCnt%16/4)/4.f + 1.0f/4,1.0f);

				g_pD3DVtxBuffEffect->Unlock();//ロックの解除を忘れずに


			D3DXMatrixIdentity(&g_mtxworldEffect);//ボールのIdenteyをだす
			D3DXMatrixInverse(&g_mtxworldEffect,NULL,&mtxView);//逆行列
			g_mtxworldEffect._41 = 0.0f;
			g_mtxworldEffect._42 = 0.0f;
			g_mtxworldEffect._43 = 0.0f;

			g_posEffect = D3DXVECTOR3(g_EffectData[nLoop].fSizex,g_EffectData[nLoop].fSizey,g_EffectData[nLoop].fSizez);

			D3DXMatrixScaling(&mtxScl,g_posEffect.x,g_posEffect.y,g_posEffect.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxScl);//スケールの反映

			D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
			D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxRot);//回転の反映

			g_posEffect = D3DXVECTOR3(	g_EffectData[nLoop].nPosx,
									g_EffectData[nLoop].nPosy,
									g_EffectData[nLoop].nPosz);

			D3DXMatrixTranslation(&mtxTranslate,g_posEffect.x,g_posEffect.y,g_posEffect.z);//XYZ
			D3DXMatrixMultiply(&g_mtxworldEffect,&g_mtxworldEffect,&mtxTranslate);//ワールドの反映

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldEffect);

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldEffect);
			//ポリゴンの描画

			pDevice->SetStreamSource(0,g_pD3DVtxBuffEffect,0,sizeof(VERTEX_3D));//ポリゴンセット

			pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

			pDevice->SetTexture(0,g_pD3DTextureEffect);//テクスチャセット

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
											0,
											2);//個数
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
											0,
											2);//個数
		}

	}
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			//影のセット　加算処理作成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexEffect(void)
{
	VERTEX_3D *pvtx;
	g_pD3DVtxBuffEffect->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,EF_SIZEWH,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(EF_SIZEWH,EF_SIZEWH,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(EF_SIZEWH,0.0f,0);//頂点の座標格納

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,100,0,255); //色設定

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f/4,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f/4,1.0f);

g_pD3DVtxBuffEffect->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// フラグセット
//*****************************************************************************
void SetEffect(float x,float y,float z,float rot,D3DCOLOR col)
{
	for(int nLoop = 0;nLoop < EF_MAX;nLoop++)
	{
		if(g_EffectData[nLoop].nFlag != 1)//フラグが立っていないと
		{
			//フラグと座標セット
			g_EffectData[nLoop].nFlag = 1;
			g_EffectData[nLoop].nPosx = x;
			g_EffectData[nLoop].nPosy = y;
			g_EffectData[nLoop].nPosz = z;
			g_EffectData[nLoop].fSizex = 1.0f;
			g_EffectData[nLoop].fSizey = 1.0f;
			g_EffectData[nLoop].fSizez = 1.0f;
			g_EffectData[nLoop].fAlpha = 1.0f;
			g_EffectData[nLoop].nRot = rot;
			g_EffectData[nLoop].nCnt = 0;
			g_EffectData[nLoop].dCol = col;
			return;
		}
	}
}

D3DXVECTOR3 GetEffectPos(int nNum)
{
	D3DXVECTOR3 vector = D3DXVECTOR3(g_EffectData[nNum].nPosx,g_EffectData[nNum].nPosy,g_EffectData[nNum].nPosz);
	return vector;
}

//End Of File