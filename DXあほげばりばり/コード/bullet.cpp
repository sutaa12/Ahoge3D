//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "main.h"
#include "explo.h"
#include "effect.h"
#include "score.h"
#include "enemy.h"
#include "hitchk.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEX_BULLET ("data/TEXTURE/bullet000.png")
#define BULEET_RADIUS (10)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
D3DXMATRIX GetMatrixView(void);
void InitVertexBullet(void);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBullet = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX g_mtxviewBullet;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionBullet;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldBullet;		//ワールドマトリックス


D3DXVECTOR3 g_posBullet;		//地面の位置
D3DXVECTOR3 g_rotBullet;		//地面の向き
D3DXVECTOR3 g_sclBullet;		//地面の大きさ

BULLETDATA g_BulletData[B_MAX];//弾データの配列
//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//フィールド初期化
	g_posBullet = D3DXVECTOR3(0.0,50.0,0.0);
	g_rotBullet = D3DXVECTOR3(0.0,0.0,0.0);
	g_sclBullet = D3DXVECTOR3(1.0f,1.0f,1.0f);

	//頂点情報の設定
	if(FAILED(pDevice->CreateVertexBuffer(	//頂点バッファ作成
				sizeof(VERTEX_3D)*4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_pD3DVtxBuffBullet,
				NULL)))
			{
		return E_FAIL;//読み込めなかったらエラー
			}
	InitVertexBullet();

	//テクスチャ読み込み
		D3DXCreateTextureFromFile(GetDevice(),
								TEX_BULLET,
								&g_pD3DTextureBullet);
	//配列初期化
		for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{

		//フラグと座標セット
		g_BulletData[nLoop].nFlag = 0;
		g_BulletData[nLoop].fPosx = 0;
		g_BulletData[nLoop].fPosy = 0;
		g_BulletData[nLoop].fPosz = 0;
		g_BulletData[nLoop].nRot = 0;
		g_BulletData[nLoop].Col = 0;
		g_BulletData[nLoop].fPosCicle.Radius = BULEET_RADIUS;//半径セット
		g_BulletData[nLoop].fPosCicle.Pos.Position = D3DXVECTOR3(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz);

	}
	return S_OK;
}
//*****************************************************************************
// 終了処理
//*****************************************************************************
void UninitBullet(void)
{
	if(g_pD3DVtxBuffBullet!=NULL)
	{
		g_pD3DVtxBuffBullet->Release();//初期化
		g_pD3DVtxBuffBullet=NULL;
	}
		if(g_pD3DTextureBullet!=NULL)//空でないなら
	{
		g_pD3DTextureBullet->Release();
		g_pD3DTextureBullet=NULL;//アドレスを空に
	}
}
//*****************************************************************************
// 更新処理
//*****************************************************************************
void UpdateBullet(void)
{
	for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{
		if(g_BulletData[nLoop].nFlag == 1)
		{
			g_BulletData[nLoop].fPosx -= sinf(g_BulletData[nLoop].nRot)*B_SPEED;
			g_BulletData[nLoop].fPosz -= cosf(g_BulletData[nLoop].nRot)*B_SPEED;
			g_BulletData[nLoop].fPosCicle.Pos.Position = D3DXVECTOR3(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz);
			
			for(int nLoopEne = 0;nLoopEne < ENEMY_MAX;nLoopEne++)
			{
				if(GetEnemyLife(nLoopEne) == 1 && CircleHitChk(g_BulletData[nLoop].fPosCicle,GetEnemyPos(nLoopEne)))
				{
					SetEnemyLife(nLoopEne);//HPを変える
				//爆発セット
				SetExplo(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx-rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%30 - 20,g_BulletData[nLoop].fPosy-rand()%20,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				g_BulletData[nLoop].nFlag = 0;
				AddScore(20);
				}
			}
			//g_BulletData[nLoop].Col = ((g_BulletData[nLoop].Col >> 16)) - 1000;
			g_BulletData[nLoop].Col -= 100;
			SetEffect(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot,g_BulletData[nLoop].Col);
			
			//フィールドの外に出たらフラグを0にする
			if(g_BulletData[nLoop].fPosx < 0 || g_BulletData[nLoop].fPosx > FIELDSIZEMAX || g_BulletData[nLoop].fPosz < 0 || g_BulletData[nLoop].fPosz > FIELDSIZEMAX )
			{
				//爆発セット
				SetExplo(g_BulletData[nLoop].fPosx,g_BulletData[nLoop].fPosy,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx-rand()%20,g_BulletData[nLoop].fPosy+rand()%10,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				SetExplo(g_BulletData[nLoop].fPosx+rand()%30 - 20,g_BulletData[nLoop].fPosy-rand()%20,g_BulletData[nLoop].fPosz,g_BulletData[nLoop].nRot);
				g_BulletData[nLoop].nFlag = 0;
				AddScore(20);
			}
		}
	}
}
//*****************************************************************************
// 描画処理
//*****************************************************************************
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	D3DXMATRIX mtxView;
	mtxView = GetMatrixView();//カメラのビューをとってくる
	D3DXMatrixIdentity(&g_mtxworldBullet);//ボールのIdenteyをだす
	D3DXMatrixInverse(&g_mtxworldBullet,NULL,&mtxView);//逆行列
	g_mtxworldBullet._41 = 0.0f;
	g_mtxworldBullet._42 = 0.0f;
	g_mtxworldBullet._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,g_posBullet.x,g_posBullet.y,g_posBullet.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldBullet);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF,0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

	for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{
		if(g_BulletData[nLoop].nFlag == 1)//フラグが立っていないと
		{
			g_posBullet = g_BulletData[nLoop].fPosCicle.Pos.Position;

	D3DXMatrixIdentity(&g_mtxworldBullet);//ボールのIdenteyをだす
	D3DXMatrixInverse(&g_mtxworldBullet,NULL,&mtxView);//逆行列
	g_mtxworldBullet._41 = 0.0f;
	g_mtxworldBullet._42 = 0.0f;
	g_mtxworldBullet._43 = 0.0f;

	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxScl);//スケールの反映

	D3DXMatrixRotationYawPitchRoll(&mtxRot,0.0f,0.0f,0.0f);//YXZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,g_posBullet.x,g_posBullet.y,g_posBullet.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldBullet,&g_mtxworldBullet,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldBullet);

			pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldBullet);
			//ポリゴンの描画

			pDevice->SetStreamSource(0,g_pD3DVtxBuffBullet,0,sizeof(VERTEX_3D));//ポリゴンセット

			pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

			pDevice->SetTexture(0,g_pD3DTextureBullet);//テクスチャセット

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの設定
											0,
											2);//個数
		}

	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
}
//*****************************************************************************
// 頂点初期化
//*****************************************************************************
void InitVertexBullet(void)
{
	VERTEX_3D *pvtx;
	g_pD3DVtxBuffBullet->Lock(0,0,(void**)&pvtx,0);//頂点の位置をロック

				//頂点情報の初期化
	pvtx[0].vtx = D3DXVECTOR3(0,B_SIZEWH,0.0f);//頂点の座標格納
	pvtx[1].vtx = D3DXVECTOR3(B_SIZEWH,B_SIZEWH,0.0f);//頂点の座標格納
	pvtx[2].vtx = D3DXVECTOR3(0,0.0f,0);//頂点の座標格納
	pvtx[3].vtx = D3DXVECTOR3(B_SIZEWH,0.0f,0);//頂点の座標格納

	pvtx[0].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[1].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[2].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納
	pvtx[3].nor = D3DXVECTOR3(0.0f,0.0f,-1.0f);//法線の座標格納

	pvtx[0].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[1].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[2].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定
	pvtx[3].diffuse = D3DCOLOR_RGBA(255,255,255,255); //色設定

	pvtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pvtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pvtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pvtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

g_pD3DVtxBuffBullet->Unlock();//ロックの解除を忘れずに

}
//*****************************************************************************
// フラグセット
//*****************************************************************************
void SetShot(float x,float y,float z,float rot)
{
	for(int nLoop = 0;nLoop < B_MAX;nLoop++)
	{
		if(g_BulletData[nLoop].nFlag != 1)//フラグが立っていないと
		{
			//フラグと座標セット
			g_BulletData[nLoop].nFlag = 1;
			g_BulletData[nLoop].fPosx = x;
			g_BulletData[nLoop].fPosy = y;
			g_BulletData[nLoop].fPosz = z;
			g_BulletData[nLoop].nRot = rot;
			g_BulletData[nLoop].Col = D3DCOLOR_RGBA(rand()%255,rand()%255,rand()%255,0);
			return;
		}
	}
}

int GetBulletFlag(int nNum)
{
	return g_BulletData[nNum].nFlag;
}
D3DXVECTOR3 GetBulletPos(int nNum)
{
	D3DXVECTOR3 vector = D3DXVECTOR3(g_BulletData[nNum].fPosx,g_BulletData[nNum].fPosy,g_BulletData[nNum].fPosz);
	return vector;
}

//End Of File