//=============================================================================
//
// �����\������ [message.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMessageProc(void);
void UninitMessageProc(void);
void UpdateMessageProc(void);
void DrawMessageProc(void);

void PrintMessageProc(const char *fmt,...);

#endif