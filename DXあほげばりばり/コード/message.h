//=============================================================================
//
// 文字表示処理 [message.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMessageProc(void);
void UninitMessageProc(void);
void UpdateMessageProc(void);
void DrawMessageProc(void);

void PrintMessageProc(const char *fmt,...);

#endif