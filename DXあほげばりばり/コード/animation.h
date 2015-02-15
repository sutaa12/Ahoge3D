//=============================================================================
//
// アニメーション処理処理 [animation.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "main.h"
#include<vector>
//*****************************************************************************
// インターフェース生成
//*****************************************************************************
using namespace std;
interface IHighLevelAnimController
{
public:
	//アニメーションコントローラー設定
	bool SetAnimationController(ID3DXAnimationController *pAnimCont);
	//アニメーションコントローラー取得
	bool GetAnimationController(ID3DXAnimationController **ppAnimCont);
	//ループ時間設定
	bool SetLoopTime(UINT animID,FLOAT time);
	//動作開始にかかる時間設定
	bool SetSHiftTime(UINT animID,FLOAT interval);
	//アニメーション切り替え
	bool ChangeAnimation(UINT animID);
	//アニメーション更新
	bool AdvanceTime(FLOAT time);
};
struct HLANIMATION_DESK
{
	UINT uiAnimID;//アニメーションID
	ID3DXAnimationSet *pAnimSet;//アニメーションセット
	FLOAT fLoopTime;//1ループにかかる時間
	FLOAT fTrackSpeed;	//トラックスピード調整池
	FLOAT fShiftTime;	//シフトにかかる時間
	FLOAT fCurWeightTime;//現在の重みの時間
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

		//アニメーションコントローラー設定
	bool SetAnimationController(ID3DXAnimationController *pAnimCont);
	//アニメーションコントローラー取得
	bool GetAnimationController(ID3DXAnimationController **ppAnimCont);
	//ループ時間設定
	bool SetLoopTime(UINT animID,FLOAT time);
	//動作開始にかかる時間設定
	bool SetSHiftTime(UINT animID,FLOAT interval);
	//アニメーション切り替え
	bool ChangeAnimation(UINT animID);
	//アニメーション更新
	bool AdvanceTime(FLOAT time);
	//領域解放
	void Release(void);
};
#endif