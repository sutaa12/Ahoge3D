//=============================================================================
//
// シーン繊維 [scene.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
typedef enum
{
	MODE_TITLE = 0,
	MODE_GAMEMAIN,
	MODE_RESULT,
	MODE_RANKING,
	MODE_FADE,
	MODE_NONE,
	MODE_MAX
}MODE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitScene(void);
void	UpdateScene(void);
void DrawScene(void);
void UninitScene(void);
void SetNextScene(MODE nMode);

#endif

//End Of File