//=============================================================================
//
// 当たり判定 [hitchk.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "hitchk.h"
//=============================================================================
//球の当たり判定
//=============================================================================

int CircleHitChk(PosCircle Object1,PosCircle Object2)
{
	//距離を算出
	D3DXVECTOR3 Length;
	Length = Object1.Pos.Position - Object2.Pos.Position;

	float fLength = D3DXVec3Length(&Length);

	//2つの距離が2つの半径の合計より小さかったら
	if(fLength < (Object1.Radius + Object2.Radius))
	{
		return 1;//当たった
	}

	return 0;//当たっていない
}
