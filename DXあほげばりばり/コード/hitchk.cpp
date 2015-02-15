//=============================================================================
//
// “–‚½‚è”»’è [hitchk.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "hitchk.h"
//=============================================================================
//‹…‚Ì“–‚½‚è”»’è
//=============================================================================

int CircleHitChk(PosCircle Object1,PosCircle Object2)
{
	//‹——£‚ğZo
	D3DXVECTOR3 Length;
	Length = Object1.Pos.Position - Object2.Pos.Position;

	float fLength = D3DXVec3Length(&Length);

	//2‚Â‚Ì‹——£‚ª2‚Â‚Ì”¼Œa‚Ì‡Œv‚æ‚è¬‚³‚©‚Á‚½‚ç
	if(fLength < (Object1.Radius + Object2.Radius))
	{
		return 1;//“–‚½‚Á‚½
	}

	return 0;//“–‚½‚Á‚Ä‚¢‚È‚¢
}
