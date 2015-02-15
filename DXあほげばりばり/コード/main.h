//=============================================================================
//
// メイン処理 [main.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)    // 警告対策用
#include "dinput.h"
#include "debugproc.h"
#include "message.h"
#include"xaudio2.h"
//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(800)				// ウインドウの幅
#define SCREEN_HEIGHT	(600)				// ウインドウの高さ
#define FIELDSIZEMAX (2000)
#define FILE_R ("data/SAVE/ranking.bin")
// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

typedef struct
{

	D3DXVECTOR3 Position;//現在の座標
	D3DXVECTOR3 PositionDest;//移動量分の座標
	D3DXVECTOR3 Rot;//向き
	D3DXVECTOR3 RotDest;//回転の移動量
	D3DXVECTOR3 Size;//大きさ
	D3DXVECTOR3 SizeDest;//大きさの遷移
	float fSpeed;//加速度
	int nLife;//HP
	int nCnt;//フレームカウント
	D3DCOLOR Col;//色データ

}PosBase;
typedef struct
{
	PosBase Pos;//位置
	float Radius;//半径
}PosCircle;
typedef struct
{
	D3DXMATRIX mtxview;		//ビューマトリックス
	D3DXMATRIX mtxprojection;	//プロジェクションマトリックス
	D3DXMATRIX mtxworld;		//ワールドマトリックス

}MtxSet;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT	GetFont(void);
#endif