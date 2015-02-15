//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "wall.h"
#include "effect.h"
#include "camera.h"
#include "scene.h"
#include "xallocatehierachy.h"
#include "animation.h"
#include "explo.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MODEL ("data/MODEL/sintea.x")
#define PLAYER_TEX ("data/TEXTURE/playerskin.bmp")
#define X_POS (FIELDSIZEMAX/2)
#define Y_POS (30)
#define Z_POS (FIELDSIZEMAX/2)
#define X_ROT (0.0f)
#define Y_ROT (0.0f)
#define Z_ROT (0.0f)
#define MOVE_ROT (0.1f)
#define MOVE_SPEED (0.1f)
#define MOVE_POS (5)
#define MOVE_POSSPEED (0.08f)
#define PI_90 (D3DX_PI/2)
#define PI_45 (D3DX_PI/4)
#define PLAYER_RADIUS (5)
#define PLAYER_INRAD (500)
#define PLAYER_OUTRAD (200)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetResultMode(int nMode);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;		// テクスチャへのポインタ
PLAYERDATA PlayerData;
CAllocateHierarchy g_cPlayer;

D3DXMATRIX g_mtxviewM;		//ビューマトリックス
D3DXMATRIX g_mtxprojectionM;	//プロジェクションマトリックス
D3DXMATRIX g_mtxworldM;		//ワールドマトリックス

LPD3DXMESH g_pD3DXMeshPlayer;				//メッシュ情報へのポインタ
LPD3DXBUFFER g_pD3DXBuffMatPlayer;			//マテリアル情報へのポインタ
DWORD g_nNumMatPlayer;						//マテリアル情報数
float g_PosPlayerSpeedX;						//モデルの加速度
float g_PosPlayerSpeedZ;						//モデルの加速度
CHighLevelAnimController g_Panim;
D3DXVECTOR3 g_PosPlayerSpeed;
int g_nkeypcnt;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	PlayerData.fPosCicle.Pos.Rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	PlayerData.fPosCicle.Pos.RotDest = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
	PlayerData.fPosCicle.Pos.nCnt = 0;
	PlayerData.fPosCicle.Pos.Position = D3DXVECTOR3(X_POS,Y_POS,Z_POS);
	PlayerData.fPosCicle.Pos.PositionDest = D3DXVECTOR3(X_POS,Y_POS,Z_POS);
	g_PosPlayerSpeed.x = 0;
	g_PosPlayerSpeed.z = 0;
	g_PosPlayerSpeed = D3DXVECTOR3(X_POS,Y_POS,Z_POS);
	PlayerData.fPosCicle.Pos.Col = 0;
	PlayerData.fPosCicle.Pos.nLife = 5;
	PlayerData.AttackFlag = 0;
	PlayerData.fINRad = PLAYER_INRAD;
	PlayerData.fOUTRad = PLAYER_OUTRAD;
	PlayerData.bHit = false;
	PlayerData.bPose = false;
	g_cPlayer.Load(pDevice,PLAYER_MODEL,PLAYER_TEX);
	g_Panim.SetAnimationController(g_cPlayer.GetAnimeCtrl());
	g_nkeypcnt = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if(g_pD3DXBuffMatPlayer!=NULL)
	{
		g_pD3DXBuffMatPlayer->Release();//初期化
		g_pD3DXBuffMatPlayer=NULL;
	}

	if(g_pD3DXMeshPlayer!=NULL)
	{
		g_pD3DXMeshPlayer->Release();//初期化
		g_pD3DXMeshPlayer=NULL;
	}

		if(g_pD3DTexturePlayer!=NULL)//空でないなら
	{
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer=NULL;//アドレスを空に
	}

	g_Panim.Release();
	g_cPlayer.Release();
		
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	PrintMessageProc("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n残りHP%d\n",PlayerData.fPosCicle.Pos.nLife);
	PlayerData.fPosCicle.Pos.RotDest.y=PlayerData.fPosCicle.Pos.Rot.y;
	if(PlayerData.fPosCicle.Pos.nLife == 0 )
	{
		SetNextScene(MODE_RESULT);
		SetResultMode(0);
	}else{
		PlayerData.AttackFlag = P_ACT_WAIT;
		if(GetKeyboardTrigger(DIK_P))
		{
			StopSound(SOUND_LABEL_SE_HIKIYOSE);
			StopSound(SOUND_LABEL_SE_ATTACK);
			PlayerData.bPose =true;
		}
		if(GetKeyboardTrigger(DIK_SPACE))
		{
			g_nkeypcnt = 0;
		PlaySound(SOUND_LABEL_SE_HIKIYOSE);
		}
		if(GetKeyboardPress(DIK_SPACE))
		{
			SetEffect(PlayerData.fPosCicle.Pos.Position.x + rand()%60-30,PlayerData.fPosCicle.Pos.Position.y + rand()%60-30,PlayerData.fPosCicle.Pos.Position.z + rand()%60-30,PlayerData.fPosCicle.Pos.Rot.y,D3DCOLOR_RGBA(100,100,255,255));
			g_Panim.AdvanceTime(0.01f);
			PlayerData.AttackFlag = P_ACT_IN;
			g_Panim.ChangeAnimation(0);
			g_nkeypcnt++;
			if(g_cPlayer.GetAnimeCtrl()->GetTime() > 40)
			{
				g_Panim.ChangeAnimation(1);
			}
		}
		else
		if(GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
		{
		g_Panim.ChangeAnimation(2);
		g_Panim.AdvanceTime(0.04f);
		
		}else{
		g_Panim.ChangeAnimation(3);
		g_Panim.AdvanceTime(0.01f);
		}
		

		if(GetKeyboardRelease(DIK_SPACE) && g_nkeypcnt >= 10)
		{
			g_nkeypcnt = 0;
			StopSound(SOUND_LABEL_SE_HIKIYOSE);
			PlaySound(SOUND_LABEL_SE_ATTACK);
			PlayerData.AttackFlag = P_ACT_OUT;
			SetExplo(PlayerData.fPosCicle.Pos.Position.x + rand()%60-30,PlayerData.fPosCicle.Pos.Position.y + rand()%60-30,PlayerData.fPosCicle.Pos.Position.z + rand()%60-30,PlayerData.fPosCicle.Pos.Rot.y);
		}

		if(!GetKeyboardPress(DIK_SPACE))
		{

		if(GetKeyboardPress(DIK_W))
		{
			g_PosPlayerSpeed.x=g_PosPlayerSpeed.x-sinf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
			g_PosPlayerSpeed.z=g_PosPlayerSpeed.z-cosf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
		}

		if(GetKeyboardPress(DIK_S))
		{
			g_PosPlayerSpeed.x=g_PosPlayerSpeed.x+sinf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
			g_PosPlayerSpeed.z=g_PosPlayerSpeed.z+cosf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
		}

		if(GetKeyboardPress(DIK_D))
		{
			g_PosPlayerSpeed.x=g_PosPlayerSpeed.x-cosf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
			g_PosPlayerSpeed.z=g_PosPlayerSpeed.z+sinf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;

		}

		if(GetKeyboardPress(DIK_A))
		{
			g_PosPlayerSpeed.x=g_PosPlayerSpeed.x+cosf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
			g_PosPlayerSpeed.z=g_PosPlayerSpeed.z-sinf(PlayerData.fPosCicle.Pos.Rot.y)*MOVE_POS;
		}

				if(GetKeyboardPress(DIK_E))
		{
			PlayerData.fPosCicle.Pos.RotDest.y += D3DX_PI / 6;
		}else
		if(GetKeyboardPress(DIK_Q))
		{
			PlayerData.fPosCicle.Pos.RotDest.y -= D3DX_PI / 6;
		}
		}

	
		if(PlayerData.fPosCicle.Pos.Position.x > FIELDSIZEMAX)
		{
			PlayerData.fPosCicle.Pos.Position.x = FIELDSIZEMAX;
			PlayerData.fPosCicle.Pos.PositionDest.x = 0;
			g_PosPlayerSpeed.x = PlayerData.fPosCicle.Pos.Position.x;
		}else
		if(PlayerData.fPosCicle.Pos.Position.x < 0)
		{
			PlayerData.fPosCicle.Pos.Position.x = 0;
			PlayerData.fPosCicle.Pos.PositionDest.x = 0;
			g_PosPlayerSpeed.x = PlayerData.fPosCicle.Pos.Position.x;
		}	

		if(PlayerData.fPosCicle.Pos.Position.z > FIELDSIZEMAX)
		{
			PlayerData.fPosCicle.Pos.Position.z = FIELDSIZEMAX;
			PlayerData.fPosCicle.Pos.PositionDest.z = 0;
			g_PosPlayerSpeed.z = PlayerData.fPosCicle.Pos.Position.z;
		}else
		if(PlayerData.fPosCicle.Pos.Position.z < 0)
		{
			PlayerData.fPosCicle.Pos.Position.z = 0;
			PlayerData.fPosCicle.Pos.PositionDest.z = 0;
			g_PosPlayerSpeed.z = PlayerData.fPosCicle.Pos.Position.z;
		}	

		if(PlayerData.fPosCicle.Pos.Position.y <= 0)
		{
			PlayerData.fPosCicle.Pos.Position.y=0;
			PlayerData.fPosCicle.Pos.PositionDest.y=0;
		}

		if(PlayerData.fPosCicle.Pos.RotDest.y>D3DX_PI)
			{
			PlayerData.fPosCicle.Pos.RotDest.y-=D3DX_PI*2;
		}else
		if(PlayerData.fPosCicle.Pos.RotDest.y<-D3DX_PI)
		{
			PlayerData.fPosCicle.Pos.RotDest.y+=D3DX_PI*2;
		}

	if(PlayerData.bHit == true)
	{
		PlayerData.fPosCicle.Pos.nCnt++;
		if(PlayerData.fPosCicle.Pos.nCnt > 60)
		{
			PlayerData.bHit = false;
			PlayerData.fPosCicle.Pos.nCnt = 0;
		}
	}

		//移動量処理
		PlayerData.fPosCicle.Pos.PositionDest.x =(g_PosPlayerSpeed.x-PlayerData.fPosCicle.Pos.Position.x)*MOVE_POSSPEED;

		PlayerData.fPosCicle.Pos.Position.x+=PlayerData.fPosCicle.Pos.PositionDest.x;

		PlayerData.fPosCicle.Pos.PositionDest.z =(g_PosPlayerSpeed.z-PlayerData.fPosCicle.Pos.Position.z)*MOVE_POSSPEED;

		PlayerData.fPosCicle.Pos.Position.z+=PlayerData.fPosCicle.Pos.PositionDest.z;
	
		PlayerData.fPosCicle.Pos.Col = rand()&65535;
	
		//ゆっくり回転
		float fDiffRotY;
		fDiffRotY = PlayerData.fPosCicle.Pos.RotDest.y - PlayerData.fPosCicle.Pos.Rot.y;

		if(fDiffRotY>D3DX_PI||fDiffRotY<-D3DX_PI)
		{
			if(fDiffRotY>D3DX_PI)
			{
				fDiffRotY-=D3DX_PI*2;
			}else
			if(fDiffRotY<-D3DX_PI)
			{
				fDiffRotY+=D3DX_PI*2;
			}
		}

		PlayerData.fPosCicle.Pos.Rot.y += fDiffRotY * MOVE_ROT;
		if(PlayerData.fPosCicle.Pos.Rot.y>D3DX_PI||PlayerData.fPosCicle.Pos.Rot.y<-D3DX_PI)
		{
			if(PlayerData.fPosCicle.Pos.Rot.y>D3DX_PI)
			{
				PlayerData.fPosCicle.Pos.Rot.y-=D3DX_PI*2;
			}else
			if(PlayerData.fPosCicle.Pos.Rot.y<-D3DX_PI)
			{
				PlayerData.fPosCicle.Pos.Rot.y+=D3DX_PI*2;
			}
		}
	
	}

	#ifdef _DEBUG
	PrintDebugProc("モデル座標(X:%.2fY:%.2fZ:%.2f)\n",PlayerData.fPosCicle.Pos.Position.x,PlayerData.fPosCicle.Pos.Position.y,PlayerData.fPosCicle.Pos.Position.z);
	PrintDebugProc("モデル向き(%.3f),速度(X:%.2f,Z:%.2f)\n",PlayerData.fPosCicle.Pos.Rot.y,PlayerData.fPosCicle.Pos.PositionDest.x,PlayerData.fPosCicle.Pos.PositionDest.z);
	
	#endif
}



//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;	//デフォルトマテリアル
	D3DMATERIAL9 matDef;	//退避用マテリアル
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	if(PlayerData.bHit == false || PlayerData.fPosCicle.Pos.nCnt %2 == 1)
	{
	//ブロジェクトマトリックス
	D3DXMatrixIdentity(&g_mtxworldM);
	D3DXMatrixScaling(&mtxScl,0.2f,0.2f,0.2f);//XYZ
	D3DXMatrixMultiply(&g_mtxworldM,&g_mtxworldM,&mtxScl);//スケールの反映
	//ワールドマトリックス
	D3DXMatrixRotationYawPitchRoll(&mtxRot,PlayerData.fPosCicle.Pos.Rot.y,PlayerData.fPosCicle.Pos.Rot.x,PlayerData.fPosCicle.Pos.Rot.z);//YXZ
	D3DXMatrixMultiply(&g_mtxworldM,&g_mtxworldM,&mtxRot);//回転の反映

	D3DXMatrixTranslation(&mtxTranslate,PlayerData.fPosCicle.Pos.Position.x,PlayerData.fPosCicle.Pos.Position.y,PlayerData.fPosCicle.Pos.Position.z);//XYZ
	D3DXMatrixMultiply(&g_mtxworldM,&g_mtxworldM,&mtxTranslate);//ワールドの反映

	pDevice->SetTransform(D3DTS_WORLD,&g_mtxworldM);

	g_cPlayer.SetWorld(&g_mtxworldM);
	pDevice->SetTexture(0,g_pD3DTexturePlayer);

	g_cPlayer.Draw(pDevice);
	}
}
D3DXVECTOR3 PlayerRot(void)
{
	return PlayerData.fPosCicle.Pos.Rot;
}

D3DXVECTOR3 GetPlayerPosition(void)
{
	return PlayerData.fPosCicle.Pos.Position;
}

D3DXVECTOR3 GetPlayerSpeed(void)
{
	return PlayerData.fPosCicle.Pos.PositionDest;
}

PosCircle GetPlayer(void)
{
	return PlayerData.fPosCicle;
}
void SetPlayerFlag(void)
{
	PlayerData.fPosCicle.Pos.nLife--;
	PlayerData.bHit = true;
	PlayerData.fPosCicle.Pos.nCnt = 0;
}
int GetPlayerFlag(void)
{
	return PlayerData.AttackFlag;
}
PosCircle GetAttackInCircle(void)
{
	PosCircle Pos = PlayerData.fPosCicle;
	Pos.Radius = PlayerData.fINRad;
	return Pos;
}
PosCircle GetAttackOutCircle(void)
{
	PosCircle Pos = PlayerData.fPosCicle;
	Pos.Radius = PlayerData.fOUTRad;
	return Pos;
}

bool GetHitlPayer(void)
{
	return PlayerData.bHit;

}

bool GetPosePlayer(void)
{
	return PlayerData.bPose;

}

void SetPosePlayer(bool bChoice)
{
	PlayerData.bPose = bChoice;

}
//End Of FIle