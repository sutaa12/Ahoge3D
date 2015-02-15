//=============================================================================
//
// ���C������ [main.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "d3dx9.h"

#define DIRECTINPUT_VERSION (0x0800)    // �x���΍��p
#include "dinput.h"
#include "debugproc.h"
#include "message.h"
#include"xaudio2.h"
//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(800)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(600)				// �E�C���h�E�̍���
#define FIELDSIZEMAX (2000)
#define FILE_R ("data/SAVE/ranking.bin")
// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

typedef struct
{

	D3DXVECTOR3 Position;//���݂̍��W
	D3DXVECTOR3 PositionDest;//�ړ��ʕ��̍��W
	D3DXVECTOR3 Rot;//����
	D3DXVECTOR3 RotDest;//��]�̈ړ���
	D3DXVECTOR3 Size;//�傫��
	D3DXVECTOR3 SizeDest;//�傫���̑J��
	float fSpeed;//�����x
	int nLife;//HP
	int nCnt;//�t���[���J�E���g
	D3DCOLOR Col;//�F�f�[�^

}PosBase;
typedef struct
{
	PosBase Pos;//�ʒu
	float Radius;//���a
}PosCircle;
typedef struct
{
	D3DXMATRIX mtxview;		//�r���[�}�g���b�N�X
	D3DXMATRIX mtxprojection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxworld;		//���[���h�}�g���b�N�X

}MtxSet;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT	GetFont(void);
#endif