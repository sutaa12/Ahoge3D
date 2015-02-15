//=============================================================================
//
// アニメーションコントローラー [oxallocatehierachy.h]
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
 * アニメーション階層メッシュ読み込みクラス
 */
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
private:
    // メッシュフレーム構造体
    struct CMeshFrame: public D3DXFRAME
    {
        D3DXMATRIX CombinedTransformationMatrix;
    };
    // メッシュコンテナ構造体
    struct CMeshContainer: public D3DXMESHCONTAINER
    {
        LPDIRECT3DTEXTURE9*  ppTextures;
        DWORD weight;              // 重みの個数（重みとは頂点への影響。）
        DWORD cntBone;             // ボーンの数
        LPD3DXBUFFER pBoneBuffer;  // ボーンテーブル
        D3DXMATRIX** ppMatRoot;    // 全てのボーンのワールド行列の先頭ポインタ
        D3DXMATRIX* pMatOffset;    // ボーンのオフセット行列
    };
    LPD3DXFRAME               m_pFrameRoot; // フレームのトップノード
    ID3DXAnimationController *m_pAnimeCtrl; // アニメーションコントローラ
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