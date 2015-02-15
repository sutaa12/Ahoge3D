//=============================================================================
//
// ランキング [Ranking.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <stdio.h>
#include "main.h"
#include "message.h"
#include "rankingmain.h"
#include "scene.h"
#include "titlestart.h"
#include "titlelogo.h"
#include "input.h"
#include "titleplayer.h"
#include "light.h"
#include "titlecamera.h"
#include "sound.h"
#include "meshdoom.h"
//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define RANK_MAX (5)//ランキング
#define RANK_NX (300)
#define RANK_NY (130)
#define RANK_MAX_M (30)
#define RANK_SIZE (1000)
#define RANKL_X (220)
#define RANKL_Y (50)
#define RANKL_W (220)
#define RANKL_H (50)

//*****************************************************************************
	int RankNum[RANK_MAX];
	
	int RnakCnt;

//*****************************************************************************
//処理
//*****************************************************************************
HRESULT RankingInit(void)
{
	//読み込み
	FILE *fp;
	fp=fopen(FILE_R,"rb");
	if(fp!=NULL)
	{
		int Seek=0;
		int *p;
		fseek(fp,0,SEEK_END);
		Seek=ftell( fp)/4;
		fseek(fp,0,SEEK_SET);
		p = new int[Seek];
		fread(p,sizeof(int),Seek,fp);
		fclose(fp);
		int Temp;
		//並べ替え
		if(Seek>=1)
		{
		for(int nLoop=0;nLoop<Seek-1;nLoop++)
		{
			for(int nLoop2=Seek-1;nLoop2>nLoop;nLoop2--)
			{
				if(p[nLoop2]>p[nLoop2-1])
				{
					Temp = p[nLoop2];
					p[nLoop2] = p[nLoop2-1];
					p[nLoop2-1] = Temp;
				}
			}
		}
		}
			for(int nLoop=0;nLoop<RANK_MAX;nLoop++)
		{
		
		RankNum[nLoop]=p[nLoop];
		if(RankNum[nLoop] <0)
		{
			RankNum[nLoop] = 0;	
		}
		}
		delete[] p;
		

	}
	InitMeshDoom(14,14,4000,4000);
	InitLight();
	InitTitleCamera();
	InitTitlePlayer();
	InitTitleLogo();
	PlaySound(SOUND_LABEL_BGM000);
		return S_OK;
}
void RankingUninit(void)
{
	UninitMeshDoom();
	UninitLight();
	UninitTitleCamera();
	UninitTitlePlayer();
	UninitTitleLogo();
	StopSound();
}
void RankingUpdate(void)
{
	RnakCnt++;
	PrintMessageProc("\n\n\n\n\n\n\n\nランキング\n");
			for(int nLoop=0;nLoop<RANK_MAX;nLoop++)
	{
		PrintMessageProc("%d位:%d\n",nLoop+1,RankNum[nLoop]);
	}
	if(GetKeyboardTrigger(DIK_RETURN)||RnakCnt>RANK_MAX_M*60)
	{
		SetNextScene(MODE_TITLE);
	}
	UpdateMeshDoom();
	UpdateLight();
	UpdateTitleCamera();
	UpdateTitlePlayer();
	UpdateTitleLogo();
}

void RankingDraw(void)
{
	DrawMeshDoom();
	DrawTitleLogo();

	SetTitleCamera();
	DrawTitlePlayer();

}