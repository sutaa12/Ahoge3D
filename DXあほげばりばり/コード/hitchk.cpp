//=============================================================================
//
// �����蔻�� [hitchk.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "hitchk.h"
//=============================================================================
//���̓����蔻��
//=============================================================================

int CircleHitChk(PosCircle Object1,PosCircle Object2)
{
	//�������Z�o
	D3DXVECTOR3 Length;
	Length = Object1.Pos.Position - Object2.Pos.Position;

	float fLength = D3DXVec3Length(&Length);

	//2�̋�����2�̔��a�̍��v��菬����������
	if(fLength < (Object1.Radius + Object2.Radius))
	{
		return 1;//��������
	}

	return 0;//�������Ă��Ȃ�
}
