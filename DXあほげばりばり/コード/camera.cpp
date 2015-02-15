//=============================================================================
//
// �J�������� [camera.cpp]
// Author : NARITADA SUZUKI
//
//=============================================================================
#include "camera.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define C_MOVEX (1)
#define C_MOVEY (1)
#define C_MOVEZ (1)
#define C_ROTMOVE (0.01f)
#define C_POSY (80)
#define C_MODELPOS (160)
#define C_SPEED (0.15f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3 g_poscameraP;	//�J�����̈ʒu
D3DXVECTOR3 g_poscameraR;	//�J�����̒����_
D3DXVECTOR3 g_veccameraV;	//�J�����̕����x�N�g��

D3DXVECTOR3 g_poscameraPDest;	//�ړI�̃J�����̈ʒu
D3DXVECTOR3 g_poscameraRDest;	//�ړI�̃J�����̒����_

D3DXMATRIX g_mtxview;		//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxprojection;	//�v���W�F�N�V�����}�g���b�N�X
D3DXMATRIX g_mtxworld;		//���[���h�}�g���b�N�X


float g_veccamera;	//�J���������_�܂ł̋���
D3DXVECTOR3 g_rotcamera;	//�J�����̌���


//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitCamera(void)
{
	D3DXVECTOR3 PlayerPos = GetPlayerPosition();

	g_rotcamera = D3DXVECTOR3(0.0f,0.0f,0.0f);
	//���f���̈ʒu����Z�����Ɉ�苗��
	g_poscameraP = D3DXVECTOR3(	PlayerPos.x,
									PlayerPos.y+C_POSY,
									PlayerPos.z-C_MODELPOS);
	//���f���̑O���̈ʒu
	g_poscameraR = D3DXVECTOR3(	PlayerPos.x-sinf(PlayerRot().y)*g_veccamera,
									PlayerPos.y,
									PlayerPos.z+cosf(PlayerRot().y)*g_veccamera);

	//�J�����̖@���ݒ�
	g_veccameraV = D3DXVECTOR3(0.0f,1.0f,0.0f);
	//�J�����̒����_�܂ł̋����ݒ�
	g_veccamera = 5;
	g_poscameraR = D3DXVECTOR3(g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera,5.0f,g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera);
	g_mtxview ;		//�r���[�}�g���b�N�X
	g_mtxprojection;	//�v���W�F�N�V�����}�g���b�N�X
	g_mtxworld;		//���[���h�}�g���b�N�X
	//�J�����̈ʒu�̍����ݒ�	
	g_poscameraPDest = D3DXVECTOR3(0.0f,C_POSY,50.0f);
	//�J�����̉�]�̍����ݒ�
	g_poscameraRDest = D3DXVECTOR3(g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera,5.0f,g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera);
	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	
	//���f���̈ʒu�擾
	D3DXVECTOR3 PlayerPos = GetPlayerPosition();


		if(GetKeyboardPress(DIK_SPACE))
	{
		if(GetKeyboardTrigger(DIK_SPACE))
		{
		g_poscameraPDest = D3DXVECTOR3(	g_poscameraRDest.x + sinf(g_rotcamera.y)* (C_MODELPOS*1.5),
									g_poscameraRDest.y+C_POSY*3,
										g_poscameraRDest.z + cosf(g_rotcamera.y) * (C_MODELPOS*1.5f));
		}
		if(GetKeyboardPress(DIK_Q))
	{
		g_rotcamera.y+= D3DX_PI /90;
		g_poscameraPDest = D3DXVECTOR3(	g_poscameraRDest.x + sinf(g_rotcamera.y)* (C_MODELPOS*1.5),
								g_poscameraRDest.y+C_POSY*2,
									g_poscameraRDest.z + cosf(g_rotcamera.y) * (C_MODELPOS*1.5f));
	}else
	if(GetKeyboardPress(DIK_E))
	{
	//���f���̈ʒu����Z�����Ɉ�苗��
	g_rotcamera.y-= D3DX_PI /90;
	g_poscameraPDest = D3DXVECTOR3(	g_poscameraRDest.x + sinf(g_rotcamera.y)* (C_MODELPOS*1.5),
								g_poscameraRDest.y+C_POSY*2,
									g_poscameraRDest.z + cosf(g_rotcamera.y) * (C_MODELPOS*1.5));
	}
	//���f���̈ʒu����Z�����Ɉ�苗��
	g_poscameraP = D3DXVECTOR3(	g_poscameraP.x+(g_poscameraPDest.x-g_poscameraP.x)*C_SPEED,
								g_poscameraP.y+(g_poscameraPDest.y-g_poscameraP.y)*C_SPEED,
								g_poscameraP.z+(g_poscameraPDest.z-g_poscameraP.z)*C_SPEED);


		}else{
	//���f���̑O���̈ʒu
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*C_SPEED,
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*C_SPEED);
	if(GetKeyboardPress(DIK_W)||GetKeyboardPress(DIK_A)||GetKeyboardPress(DIK_D)||GetKeyboardPress(DIK_S))
	{
	//���f���̑O���̈ʒu
	g_poscameraRDest = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*(C_SPEED*1.5f),
									g_poscameraR.y,
									g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*(C_SPEED*1.5f));
		//���f���̑O���̈ʒu
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*(C_SPEED*1.5f),
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*(C_SPEED*1.5f));

	}
	
	//���f���̉����x�擾
	D3DXVECTOR3 Playerspeed = GetPlayerSpeed();
	g_rotcamera.y = PlayerRot().y;

	//���f���̈ʒu����Z�����Ɉ�苗��
	g_poscameraPDest = D3DXVECTOR3(	PlayerPos.x - sinf(PlayerRot().y)* (-C_MODELPOS),
								PlayerPos.y+C_POSY,
									PlayerPos.z - cosf(PlayerRot().y) * (-C_MODELPOS));
	//���f���̑O���̈ʒu
	g_poscameraRDest = D3DXVECTOR3(	PlayerPos.x-sinf(PlayerRot().y)*g_veccamera,
									PlayerPos.y,
									PlayerPos.z-cosf(PlayerRot().y)*g_veccamera);

		//���f���̈ʒu����Z�����Ɉ�苗��
	g_poscameraP = D3DXVECTOR3(	g_poscameraP.x+(g_poscameraPDest.x-g_poscameraP.x)*C_SPEED,
								g_poscameraP.y+(g_poscameraPDest.y-g_poscameraP.y)*C_SPEED,
								g_poscameraP.z+(g_poscameraPDest.z-g_poscameraP.z)*C_SPEED);

	//���f���̑O���̈ʒu
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*C_SPEED,
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*C_SPEED);
	if(GetKeyboardPress(DIK_W)||GetKeyboardPress(DIK_A)||GetKeyboardPress(DIK_D)||GetKeyboardPress(DIK_S))
	{
	//���f���̑O���̈ʒu
	g_poscameraRDest = D3DXVECTOR3(	PlayerPos.x-sinf(PlayerRot().y)*g_veccamera+Playerspeed.x,
									PlayerPos.y,
									PlayerPos.z-cosf(PlayerRot().y)*g_veccamera+Playerspeed.z);
		//���f���̑O���̈ʒu
	g_poscameraR = D3DXVECTOR3(	g_poscameraR.x+(g_poscameraRDest.x-g_poscameraR.x)*(C_SPEED*1.5f),
								g_poscameraRDest.y,
								g_poscameraR.z+(g_poscameraRDest.z-g_poscameraR.z)*(C_SPEED*1.5f));
	}
	g_rotcamera.y = PlayerRot().y;


	}
	if(g_rotcamera.y>=D3DX_PI||g_rotcamera.y<-D3DX_PI)
	{
		g_rotcamera.y*=-1;
	}

	#ifdef _DEBUG
	PrintDebugProc("�J�������W(X:%.2fY:%.2fZ:%.2f)\n",g_poscameraP.x,g_poscameraP.y,g_poscameraP.z);
	PrintDebugProc("�J���������_(X:%.2fY:%.2fZ:%.2f)\n",g_poscameraR.x,g_poscameraR.y,g_poscameraR.z);
	PrintDebugProc("�J��������(%.3f),���x(X:%.2f,Z:%.2f)\n",g_rotcamera.y,g_poscameraRDest.x,g_poscameraRDest.z);
	#endif

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	//�r���[�}�g���b�N�X
	D3DXMatrixIdentity(&g_mtxview);
	D3DXMatrixLookAtLH(&g_mtxview,&g_poscameraP,&g_poscameraR,&g_veccameraV);
	pDevice->SetTransform(D3DTS_VIEW,&g_mtxview);
	D3DXMatrixIdentity(&g_mtxprojection);
	D3DXMatrixPerspectiveFovLH(	&g_mtxprojection,//�}�g���b�N�X�쐬
								(D3DX_PI/4),//����
								(float)SCREEN_WIDTH/SCREEN_HEIGHT,//�A�X�y�N�g��
								10.0f,	//����
								10000.0f);//�`�拗��
	pDevice->SetTransform(D3DTS_PROJECTION,&g_mtxprojection);

		//�u���W�F�N�g�}�g���b�N�X
	D3DXMatrixIdentity(&g_mtxworld);
	D3DXMatrixScaling(&mtxScl,1.0f,1.0f,1.0f);//XYZ
	D3DXMatrixMultiply(&g_mtxworld,&g_mtxworld,&mtxScl);//�X�P�[���̔��f

}

//=============================================================================
// �J�����̌������擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return g_rotcamera;
}
//=============================================================================
// �J�����̍��W��n��
//=============================================================================
D3DXVECTOR3 GetCameraP(void)
{
	return g_poscameraP;
}
//=============================================================================
// �J�����𓮂���
//=============================================================================
void PoseCameUpdate(void)
{
			if(GetKeyboardPress(DIK_W))
	{

		g_poscameraP.x+=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z+=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}

	if(GetKeyboardPress(DIK_S))
	{
		g_poscameraP.x-=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z-=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}
	if(GetKeyboardPress(DIK_D))
	{
		g_poscameraP.x+=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z-=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}
	if(GetKeyboardPress(DIK_A))
	{
		g_poscameraP.x-=cosf(g_rotcamera.y)*C_MOVEX;

		g_poscameraP.z+=sinf(g_rotcamera.y)*C_MOVEX;

		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}

		if(GetKeyboardPress(DIK_Z))
	{
		g_rotcamera.y-=C_ROTMOVE;
		g_poscameraP.x=g_poscameraR.x-(g_veccamera*sinf(g_rotcamera.y));
		g_poscameraP.z=g_poscameraR.z-(g_veccamera*cosf(g_rotcamera.y));

	}

	if(GetKeyboardPress(DIK_X))
	{
		g_rotcamera.y+=C_ROTMOVE;
		g_poscameraP.x=g_poscameraR.x-(g_veccamera*sinf(g_rotcamera.y));
		g_poscameraP.z=g_poscameraR.z-(g_veccamera*cosf(g_rotcamera.y));
	}


	if(GetKeyboardPress(DIK_Q))
	{
		g_rotcamera.y-=C_ROTMOVE;
		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;

	}
	if(GetKeyboardPress(DIK_E))
	{
		g_rotcamera.y+=C_ROTMOVE;
		g_poscameraR.x=g_poscameraP.x+sinf(g_rotcamera.y)*g_veccamera;
		g_poscameraR.z=g_poscameraP.z+cosf(g_rotcamera.y)*g_veccamera;
	}
	
	if(GetKeyboardPress(DIK_Y))
	{
		g_poscameraP.y+=C_MOVEY;
		g_poscameraR.y+=C_MOVEY;
	}	

	if(GetKeyboardPress(DIK_B))
	{ 
		g_poscameraP.y-=C_MOVEY;
		g_poscameraR.y-=C_MOVEY;
	}
		if(GetKeyboardPress(DIK_U))
	{
		g_poscameraR.y += C_MOVEY;
	}	

	if(GetKeyboardPress(DIK_N))
	{ 
		g_poscameraR.y -= C_MOVEY;
	}

}
//=============================================================================
// �J�����̒����_���W��n��
//=============================================================================
D3DXVECTOR3 GetCameraR(void)
{
	return g_poscameraR;
}
D3DXMATRIX GetMatrixView()
{
	return g_mtxview;
}

//End Of File