//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
typedef struct
{
	PosCircle fPosCicle;//中心座標と半径のデータ
	MtxSet mtxset;//マトリックスデータ格納
	int AttackFlag;//攻撃フラグ
	float fINRad;	//引きつけ位置
	float fOUTRad;//吹き飛ばし可能距離
	bool bHit;//無敵かどうか
	bool bPose;//ポーズかどうか
		
}PLAYERDATA;
//マクロ定義
typedef enum
{
	P_ACT_WAIT = 0,
	P_ACT_IN,
	P_ACT_OUT,
	P_ACT_MAX
}PLAYER_ACT;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void FixPI(float fRot);
int GetPlayerFlag(void);

bool GetPosePlayer(void);
void SetPosePlayer(bool bChoice);

D3DXVECTOR3 GetPlayerPosition(void);
D3DXVECTOR3 PlayerRot(void);
D3DXVECTOR3 GetField(void);

#endif
