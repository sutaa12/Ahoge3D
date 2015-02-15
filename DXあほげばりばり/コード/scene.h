//=============================================================================
//
// �V�[���@�� [scene.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
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
// �v���g�^�C�v�錾
//*****************************************************************************
void InitScene(void);
void	UpdateScene(void);
void DrawScene(void);
void UninitScene(void);
void SetNextScene(MODE nMode);

#endif

//End Of File