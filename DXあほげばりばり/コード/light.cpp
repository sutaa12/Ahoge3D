//=============================================================================
//
// ライト処理 [light.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9 g_aLight[3];		//ライト情報

//=============================================================================
// ライトの初期化処理
//=============================================================================
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	D3DXVECTOR3 vecDir;

	ZeroMemory(&g_aLight[0],sizeof(D3DLIGHT9)*3);//ライト初期化

	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;//平行光源　種類
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	vecDir = D3DXVECTOR3(1.0f,-1.0f,-1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction,&vecDir);
	pDevice->SetLight(0,&g_aLight[0]);
	pDevice->LightEnable(0,TRUE);
	//0

	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;//平行光源　種類
	g_aLight[1].Diffuse = D3DXCOLOR(0.6f,0.6f,0.6f,1.0f);
	vecDir = D3DXVECTOR3(0.1f,-1.0f,0.9f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction,&vecDir);
	pDevice->SetLight(1,&g_aLight[1]);
	pDevice->LightEnable(1,TRUE);
	//1

	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;//平行光源　種類
	g_aLight[2].Diffuse = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	vecDir = D3DXVECTOR3(-0.9f,0.3f,-0.5f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction,&vecDir);
	pDevice->SetLight(2,&g_aLight[2]);
	pDevice->LightEnable(2,TRUE);
	//2
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);

	return S_OK;
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}

//End Of File