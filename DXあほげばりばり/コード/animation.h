//=============================================================================
//
// �A�j���[�V������������ [animation.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "main.h"
#include<vector>
//*****************************************************************************
// �C���^�[�t�F�[�X����
//*****************************************************************************
using namespace std;
interface IHighLevelAnimController
{
public:
	//�A�j���[�V�����R���g���[���[�ݒ�
	bool SetAnimationController(ID3DXAnimationController *pAnimCont);
	//�A�j���[�V�����R���g���[���[�擾
	bool GetAnimationController(ID3DXAnimationController **ppAnimCont);
	//���[�v���Ԑݒ�
	bool SetLoopTime(UINT animID,FLOAT time);
	//����J�n�ɂ����鎞�Ԑݒ�
	bool SetSHiftTime(UINT animID,FLOAT interval);
	//�A�j���[�V�����؂�ւ�
	bool ChangeAnimation(UINT animID);
	//�A�j���[�V�����X�V
	bool AdvanceTime(FLOAT time);
};
struct HLANIMATION_DESK
{
	UINT uiAnimID;//�A�j���[�V����ID
	ID3DXAnimationSet *pAnimSet;//�A�j���[�V�����Z�b�g
	FLOAT fLoopTime;//1���[�v�ɂ����鎞��
	FLOAT fTrackSpeed;	//�g���b�N�X�s�[�h�����r
	FLOAT fShiftTime;	//�V�t�g�ɂ����鎞��
	FLOAT fCurWeightTime;//���݂̏d�݂̎���
};

class CHighLevelAnimController : IHighLevelAnimController
{
private:
	
	vector<HLANIMATION_DESK> m_Anim;
	CHighLevelAnimController operator = (CHighLevelAnimController& c){return c;}
	UINT m_CurAnimID;
	UINT m_PreAnimID;
	ID3DXAnimationController *m_AnimCont;

public:
	CHighLevelAnimController(){}
	~CHighLevelAnimController(){}

		//�A�j���[�V�����R���g���[���[�ݒ�
	bool SetAnimationController(ID3DXAnimationController *pAnimCont);
	//�A�j���[�V�����R���g���[���[�擾
	bool GetAnimationController(ID3DXAnimationController **ppAnimCont);
	//���[�v���Ԑݒ�
	bool SetLoopTime(UINT animID,FLOAT time);
	//����J�n�ɂ����鎞�Ԑݒ�
	bool SetSHiftTime(UINT animID,FLOAT interval);
	//�A�j���[�V�����؂�ւ�
	bool ChangeAnimation(UINT animID);
	//�A�j���[�V�����X�V
	bool AdvanceTime(FLOAT time);
	//�̈���
	void Release(void);
};
#endif