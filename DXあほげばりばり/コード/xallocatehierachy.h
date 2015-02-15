//=============================================================================
//
// �A�j���[�V�����R���g���[���[ [oxallocatehierachy.h]
// Author : NARITADA SUZUKI
//
//=============================================================================
#ifndef _OXALLOCATE_H_
#define _OXALLOCATE_H_

#include "main.h"

#include <string>
#include <d3dx9.h>

using namespace std;

/**
 * �A�j���[�V�����K�w���b�V���ǂݍ��݃N���X
 */
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
private:
    // ���b�V���t���[���\����
    struct CMeshFrame: public D3DXFRAME
    {
        D3DXMATRIX CombinedTransformationMatrix;
    };
    // ���b�V���R���e�i�\����
    struct CMeshContainer: public D3DXMESHCONTAINER
    {
        LPDIRECT3DTEXTURE9*  ppTextures;
        DWORD weight;              // �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
        DWORD cntBone;             // �{�[���̐�
        LPD3DXBUFFER pBoneBuffer;  // �{�[���e�[�u��
        D3DXMATRIX** ppMatRoot;    // �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^
        D3DXMATRIX* pMatOffset;    // �{�[���̃I�t�Z�b�g�s��
    };
    LPD3DXFRAME               m_pFrameRoot; // �t���[���̃g�b�v�m�[�h
    ID3DXAnimationController *m_pAnimeCtrl; // �A�j���[�V�����R���g���[��
	LPDIRECT3DTEXTURE9 pTexture;
public:
    CAllocateHierarchy() {}
    STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME*);
    STDMETHOD(CreateMeshContainer)(
        THIS_ LPCTSTR Name,
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials,
        CONST D3DXEFFECTINSTANCE *pEffectInstances,
        DWORD NumMaterials, 
        CONST DWORD *pAdjacency,
        LPD3DXSKININFO pSkinInfo, 
        LPD3DXMESHCONTAINER *ppMeshContainer);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);    
	void Release(void);
    void Load(LPDIRECT3DDEVICE9 g, string filepath,string filepathTex);
    void SetWorld(LPD3DXMATRIX mWorld);
    void Draw(LPDIRECT3DDEVICE9 g);
    ID3DXAnimationController *GetAnimeCtrl() {return m_pAnimeCtrl;}
private:
    HRESULT AllocBone(LPD3DXFRAME);
    HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER);
    void UpdateWorld(LPD3DXFRAME, LPD3DXMATRIX);
    void DrawFrame(LPDIRECT3DDEVICE9, LPD3DXFRAME);
    void DrawContainer(LPDIRECT3DDEVICE9, CMeshContainer*, CMeshFrame*);
	HRESULT ReleaseBone(LPD3DXFRAME pFrame);
	
};

#endif