//=============================================================================
//
// 的処理 [enemy.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "wall.h"
#include "effect.h"
#include "camera.h"
#include "scene.h"
#include "score.h"
#include "hitchk.h"
#include "player.h"
#include "explo.h"
#include "xallocatehierachy.h"
#include "animation.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY ("data/MODEL/nezumi.x")
#define ENE_TEX ("")
#define X_POS_ENE (200)
#define Y_POS_ENE (30)
#define Z_POS_ENE (200)
#define X_ROT_ENE (0.0f)
#define Y_ROT_ENE (0.0f)
#define Z_ROT_ENE (0.0f)
#define MOVE_ROT_ENE (0.1f)
#define MOVE_SPEED_ENE (2.f)
#define MOVE_POS_ENE (10)
#define MOVE_POS_ENESPEED (0.04f)
#define ENEMY_LIFE (1)//敵のHP
#define ENEMY_RADIUS (15)//当たり判定の範囲
#define SCORE_BASE (100)//スコアの加算ベース

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetScoreNum(int nNum);
PosCircle GetPlayer(void);
void SetPlayerFlag(void);
void SetResultMode(int nMode);
PosCircle GetAttackInCircle(void);
PosCircle GetAttackOutCircle(void);
bool GetHitlPayer(void);
void SetEnemyNum(int nNum);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
ENEMY_DATA g_EnemyData[ENEMY_MAX];//エネミーの構造体データ
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemy;		// テクスチャへのポインタ

CAllocateHierarchy g_cEnemy;
CHighLevelAnimController g_EneAnim;

LPD3DXMESH g_pD3DXMeshEnemy;				//メッシュ情報へのポインタ
LPD3DXBUFFER g_pD3DXBuffMatEnemy;			//マテリアル情報へのポインタ
DWORD g_nNumMatEnemy;						//マテリアル情報数
D3DMATERIAL9 g_DiffuseHIT;//マテリアルの色設定
int g_nEneCnt;//敵の数
float g_nEnePlus;//倒した数で変化するスピード
D3DXVECTOR3 g_PosEnemySpeed;
//=======================\======================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nEneCnt = 0;
	for(int nLoop = 0;nLoop<ENEMY_MAX;nLoop++)
	{
		SetEnemy(nLoop);
	}
	g_nEnePlus =0;


	//メッシュとアニメーション初期化
	g_cEnemy.Load(pDevice,ENEMY,ENE_TEX);
	g_EneAnim.SetAnimationController(g_cEnemy.GetAnimeCtrl());
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if(g_pD3DXBuffMatEnemy!=NULL)
	{
		g_pD3DXBuffMatEnemy->Release();//初期化
		g_pD3DXBuffMatEnemy=NULL;
	}

	if(g_pD3DXMeshEnemy!=NULL)
	{
		g_pD3DXMeshEnemy->Release();//初期化
		g_pD3DXMeshEnemy=NULL;
	}

		if(g_pD3DTextureEnemy!=NULL)//空でないなら
	{
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy=NULL;//アドレスを空に
	}
		g_EneAnim.Release();
		g_cEnemy.Release();
		g_nEnePlus = 0;
}
//=============================================================================
// 敵の位置セット
//=============================================================================
void SetEnemy(int nNum)
{
	int nSetMode = rand()%4;//モード
	//四隅から出す
	switch(nSetMode)
	{
	case 0:
		g_EnemyData[nNum].fPosCicle.Pos.Position = D3DXVECTOR3(rand()%FIELDSIZEMAX,Y_POS_ENE,
															FIELDSIZEMAX);

		break;
	case 1:
		g_EnemyData[nNum].fPosCicle.Pos.Position = D3DXVECTOR3(rand()%FIELDSIZEMAX,Y_POS_ENE,
															0);
		break;
	case 2:
		g_EnemyData[nNum].fPosCicle.Pos.Position = D3DXVECTOR3(0,Y_POS_ENE,
															rand()%FIELDSIZEMAX);
		break;
	case 3:
		g_EnemyData[nNum].fPosCicle.Pos.Position = D3DXVECTOR3(FIELDSIZEMAX,Y_POS_ENE,
															rand()%FIELDSIZEMAX);
		break;
	default:
		break;
	}
	g_EnemyData[nNum].fPosCicle.Pos.PositionDest = D3DXVECTOR3(X_POS_ENE,Y_POS_ENE,Z_POS_ENE);
	
	g_EnemyData[nNum].fPosCicle.Pos.fSpeed = MOVE_SPEED_ENE + g_nEnePlus;
	g_EnemyData[nNum].fPosCicle.Pos.nLife = ENEMY_LIFE;//HPの初期値
	g_EnemyData[nNum].fPosCicle.Radius = ENEMY_RADIUS;//半径の設定
	g_EnemyData[nNum].fPosCicle.Pos.nCnt = 0;//カウントの初期値
	g_EnemyData[nNum].fPosCicle.Pos.Col = 0xfffff;//カウントの初期値
	SetEnemyNum(g_nEneCnt);
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	g_EneAnim.AdvanceTime(0.02f);
	#ifdef _DEBUG
	PrintDebugProc("残りの敵の数(%d体)\n",g_nEneCnt);	
	#endif
	for(int nLoop = 0;nLoop<ENEMY_MAX;nLoop++)
	{

		
		//敵が生きていたら
		if(g_EnemyData[nLoop].fPosCicle.Pos.nLife == 1)
		{
			//惹きつけられていたら
			if(GetPlayerFlag() == P_ACT_IN && CircleHitChk(GetAttackInCircle(),g_EnemyData[nLoop].fPosCicle))
			{
				float fRot = 0;
				fRot = atan2f((GetPlayer().Pos.Position.z - g_EnemyData[nLoop].fPosCicle.Pos.Position.z),(GetPlayer().Pos.Position.x - g_EnemyData[nLoop].fPosCicle.Pos.Position.x));
			//惹きつけられて敵が吹き飛ばし内にいたら
			if(GetPlayerFlag() == P_ACT_IN && CircleHitChk(GetAttackOutCircle(),g_EnemyData[nLoop].fPosCicle))
			{
				//少し遅くする
				g_EnemyData[nLoop].fPosCicle.Pos.Position.x=g_EnemyData[nLoop].fPosCicle.Pos.Position.x+cosf(fRot)*(1.2f + g_EnemyData[nLoop].fPosCicle.Pos.fSpeed/2);
				g_EnemyData[nLoop].fPosCicle.Pos.Position.z=g_EnemyData[nLoop].fPosCicle.Pos.Position.z+sinf(fRot)*(1.2f + g_EnemyData[nLoop].fPosCicle.Pos.fSpeed/2);

			}else{
				g_EnemyData[nLoop].fPosCicle.Pos.Position.x=g_EnemyData[nLoop].fPosCicle.Pos.Position.x+cosf(fRot)*(2 + g_EnemyData[nLoop].fPosCicle.Pos.fSpeed/2);
				g_EnemyData[nLoop].fPosCicle.Pos.Position.z=g_EnemyData[nLoop].fPosCicle.Pos.Position.z+sinf(fRot)*(2 + g_EnemyData[nLoop].fPosCicle.Pos.fSpeed/2);
			}
				SetEffect(g_EnemyData[nLoop].fPosCicle.Pos.Position.x,g_EnemyData[nLoop].fPosCicle.Pos.Position.y - 10,g_EnemyData[nLoop].fPosCicle.Pos.Position.z,g_EnemyData[nLoop].fPosCicle.Pos.Rot.y,g_EnemyData[nLoop].fPosCicle.Pos.Col);


				//惹きつけ中に話したら
				if((GetPlayer().Pos.Position.z - g_EnemyData[nLoop].fPosCicle.Pos.Position.z)==0 && (GetPlayer().Pos.Position.x - g_EnemyData[nLoop].fPosCicle.Pos.Position.x) == 0)
				{
					fRot = 0;;
				}else{
					fRot = -D3DX_PI / 2 + atan2f(-(GetPlayer().Pos.Position.z - g_EnemyData[nLoop].fPosCicle.Pos.Position.z),(GetPlayer().Pos.Position.x - g_EnemyData[nLoop].fPosCicle.Pos.Position.x));
					g_EnemyData[nLoop].fPosCicle.Pos.Rot.y = fRot;
				}
			
			}else
			//引きつけを話したら
			if(GetPlayerFlag() == P_ACT_OUT && CircleHitChk(GetAttackOutCircle(),g_EnemyData[nLoop].fPosCicle))
			{
				float nrot = D3DX_PI * 200;
				nrot = rand()%(int)nrot;
				nrot -= D3DX_PI * 100;
				g_EnemyData[nLoop].fPosCicle.Pos.Rot.y = ( nrot/ 100);
				float fRot = 0;
				fRot = atan2f((GetPlayer().Pos.Position.z - g_EnemyData[nLoop].fPosCicle.Pos.Position.z),(GetPlayer().Pos.Position.x - g_EnemyData[nLoop].fPosCicle.Pos.Position.x));
				g_EnemyData[nLoop].fPosCicle.Pos.Position.x=g_EnemyData[nLoop].fPosCicle.Pos.Position.x-cosf(fRot)*5;
				g_EnemyData[nLoop].fPosCicle.Pos.Position.z=g_EnemyData[nLoop].fPosCicle.Pos.Position.z-sinf(fRot)*5;
				SetExplo(g_EnemyData[nLoop].fPosCicle.Pos.Position.x,g_EnemyData[nLoop].fPosCicle.Pos.Position.y,g_EnemyData[nLoop].fPosCicle.Pos.Position.z,g_EnemyData[nLoop].fPosCicle.Pos.Rot.y);
				SetExplo(g_EnemyData[nLoop].fPosCicle.Pos.Position.x+rand()%50,g_EnemyData[nLoop].fPosCicle.Pos.Position.y+rand()%30,g_EnemyData[nLoop].fPosCicle.Pos.Position.z+rand()%50,g_EnemyData[nLoop].fPosCicle.Pos.Rot.y);
				g_EnemyData[nLoop].fPosCicle.Pos.fSpeed = 40;
				g_EnemyData[nLoop].fPosCicle.Pos.nLife = 0;
				D3DXVECTOR3 Length;
				Length = g_EnemyData[nLoop].fPosCicle.Pos.Position - GetPlayer().Pos.Position;
				float fLength = D3DXVec3Length(&Length);
				if(SCORE_BASE - fLength >0)
				{
					AddScore((int)((SCORE_BASE - fLength) * 10));
				}

			}else
						if(GetPlayerFlag() == P_ACT_OUT && CircleHitChk(GetAttackInCircle(),g_EnemyData[nLoop].fPosCicle))
			{
				float nrot = D3DX_PI * 200;
				nrot = rand()%(int)nrot;
				nrot -= D3DX_PI * 100;
				g_EnemyData[nLoop].fPosCicle.Pos.Rot.y = ( nrot/ 100);

			}else{
			//敵の移動処理
			g_EnemyData[nLoop].fPosCicle.Pos.Position.x -= sinf(g_EnemyData[nLoop].fPosCicle.Pos.Rot.y) * g_EnemyData[nLoop].fPosCicle.Pos.fSpeed;
			g_EnemyData[nLoop].fPosCicle.Pos.Position.z -= cosf(g_EnemyData[nLoop].fPosCicle.Pos.Rot.y) * g_EnemyData[nLoop].fPosCicle.Pos.fSpeed;
			
			}

			if(g_EnemyData[nLoop].fPosCicle.Pos.Position.x <= 0 || g_EnemyData[nLoop].fPosCicle.Pos.Position.x >= FIELDSIZEMAX
				|| g_EnemyData[nLoop].fPosCicle.Pos.Position.z <= 0 || g_EnemyData[nLoop].fPosCicle.Pos.Position.z >= FIELDSIZEMAX)
			{
				float nMaxRoot[2] = {0,FIELDSIZEMAX};
				float * fPos[2] = {&g_EnemyData[nLoop].fPosCicle.Pos.Position.x,&g_EnemyData[nLoop].fPosCicle.Pos.Position.z};
				for(int nRoot = 0; nRoot < 2; nRoot++)
				{
					if(*fPos[nRoot] < nMaxRoot[0])
					{
						*fPos[nRoot] = nMaxRoot[0];
					}else
					if(*fPos[nRoot] > nMaxRoot[1])
					{
						*fPos[nRoot] = nMaxRoot[1];
					}
				}
				float nrot = D3DX_PI * 200;
				nrot = rand()%(int)nrot;
				nrot -= D3DX_PI * 100;
				g_EnemyData[nLoop].fPosCicle.Pos.Rot.y = ( nrot/ 100);
			}

			//的にあたったか
			if(CircleHitChk(GetPlayer(),g_EnemyData[nLoop].fPosCicle) && GetHitlPayer() == false)
			{
				SetScoreNum(GetScore());
				SetPlayerFlag();
			}
		}else
		if(g_EnemyData[nLoop].fPosCicle.Pos.nLife == 0)//飛ばされてたら
		{
			if(g_EnemyData[nLoop].fPosCicle.Pos.nCnt %3 == 0)
			{
				SetEffect(g_EnemyData[nLoop].fPosCicle.Pos.Position.x + rand()%60-30,g_EnemyData[nLoop].fPosCicle.Pos.Position.y + rand()%60-30,g_EnemyData[nLoop].fPosCicle.Pos.Position.z + rand()%60-30,g_EnemyData[nLoop].fPosCicle.Pos.Rot.y,D3DCOLOR_RGBA(255,255,100,200));
			}
				float fRot = 0;
				fRot = atan2f((GetPlayer().Pos.Position.z - g_EnemyData[nLoop].fPosCicle.Pos.Position.z),(GetPlayer().Pos.Position.x - g_EnemyData[nLoop].fPosCicle.Pos.Position.x));
				g_EnemyData[nLoop].fPosCicle.Pos.Position.x=g_EnemyData[nLoop].fPosCicle.Pos.Position.x-cosf(fRot)*8;
				g_EnemyData[nLoop].fPosCicle.Pos.Position.z=g_EnemyData[nLoop].fPosCicle.Pos.Position.z-sinf(fRot)*8;
				g_EnemyData[nLoop].fPosCicle.Pos.Position.y += g_EnemyData[nLoop].fPosCicle.Pos.fSpeed;
				g_EnemyData[nLoop].fPosCicle.Pos.fSpeed*= 0.006f;
				g_EnemyData[nLoop].fPosCicle.Pos.Position.y -= 1;
				g_EnemyData[nLoop].fPosCicle.Pos.Rot.y -= 0.4f ;
				g_EnemyData[nLoop].fPosCicle.Pos.nCnt++;
				if(g_EnemyData[nLoop].fPosCicle.Pos.nCnt %60 == 0)
				{
					g_EnemyData[nLoop].fPosCicle.Pos.nLife = -1;
					g_EnemyData[nLoop].fPosCicle.Pos.nCnt = 0;
				}
		}else
		if(g_EnemyData[nLoop].fPosCicle.Pos.nLife == -1)
		{
			if(g_EnemyData[nLoop].fPosCicle.Pos.nCnt == 0)
			{
				g_nEneCnt++;
			}
			g_EnemyData[nLoop].fPosCicle.Pos.nCnt++;
			
			if(g_EnemyData[nLoop].fPosCicle.Pos.nCnt > 300)
			{
				g_nEnePlus+= 0.05f;
				if(g_nEnePlus > 12)
				{
					g_nEnePlus = 12;
				}
				SetEnemy(nLoop);
			}
		}
#ifdef _DEBUG
		if(GetKeyboardTrigger(DIK_RETURN))
		{
			SetEnemy(nLoop);
		}
#endif
	}

}



//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;	//デフォルトマテリアル
	D3DMATERIAL9 matDef;	//退避用マテリアル
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;
	for(int nLoop = 0;nLoop<ENEMY_MAX;nLoop++)
	{
		if(g_EnemyData[nLoop].fPosCicle.Pos.nLife >= 0 )
		{
			//ブロジェクトマトリックス
			D3DXMatrixIdentity(&g_EnemyData[nLoop].mtxset.mtxworld);
			D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
			D3DXMatrixMultiply(&g_EnemyData[nLoop].mtxset.mtxworld,&g_EnemyData[nLoop].mtxset.mtxworld,&mtxScl);//スケールの反映
			//ワールドマトリックス
			D3DXMatrixRotationYawPitchRoll(&mtxRot,g_EnemyData[nLoop].fPosCicle.Pos.Rot.y,g_EnemyData[nLoop].fPosCicle.Pos.Rot.x,g_EnemyData[nLoop].fPosCicle.Pos.Rot.z);//YXZ
			D3DXMatrixMultiply(&g_EnemyData[nLoop].mtxset.mtxworld,&g_EnemyData[nLoop].mtxset.mtxworld,&mtxRot);//回転の反映

			D3DXMatrixTranslation(&mtxTranslate,g_EnemyData[nLoop].fPosCicle.Pos.Position.x,g_EnemyData[nLoop].fPosCicle.Pos.Position.y,g_EnemyData[nLoop].fPosCicle.Pos.Position.z);//XYZ
			D3DXMatrixMultiply(&g_EnemyData[nLoop].mtxset.mtxworld,&g_EnemyData[nLoop].mtxset.mtxworld,&mtxTranslate);//ワールドの反映

			pDevice->SetTransform(D3DTS_WORLD,&g_EnemyData[nLoop].mtxset.mtxworld);
			//描画
			g_cEnemy.SetWorld(&g_EnemyData[nLoop].mtxset.mtxworld);
			g_cEnemy.Draw(pDevice);
		}
	}
}

D3DXVECTOR3 GetEnemyPosition(int nNum)
{
	return g_EnemyData[nNum].fPosCicle.Pos.Position;
}

//*****************************************************************************
// HPセット
//*****************************************************************************
void SetEnemyLife(int nNum)
{
	g_EnemyData[nNum].fPosCicle.Pos.nLife = 0;
	g_EnemyData[nNum].fPosCicle.Pos.nCnt = 0;

}

//*****************************************************************************
// 位置情報わたす
//*****************************************************************************
PosCircle GetEnemyPos(int nNum)
{
	return g_EnemyData[nNum].fPosCicle;
}

int GetEnemyLife(int nNum)
{
	return g_EnemyData[nNum].fPosCicle.Pos.nLife;
}

//End Of FIle