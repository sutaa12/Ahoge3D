#include "xallocatehierachy.h"
template <class T>
inline void SAFE_RELEASE(T &p)
{
    if(p)
    {
        (p)->Release();
        (p) = NULL;
    }
};
template <class T>
inline void SAFE_DELETE(T &p)
{
    if(p)
    {
        delete (p);
        (p) = NULL;
    }
};
template <class T>
inline void SAFE_DELETE_ARRAY(T &p)
{
    if(p)
    {
        delete[] (p);
        (p) = NULL;
    }
};

/**
 * メッシュ読み込み
 * @param g        描画デバイス
 * @param filepath ファイルパス
 */
void CAllocateHierarchy::Load(LPDIRECT3DDEVICE9 g, string filepath,string filepathTex)
{
    // メッシュ読み込み
    HRESULT hr = D3DXLoadMeshHierarchyFromX(
        filepath.c_str(),
        D3DXMESH_MANAGED,
        g,
        this,
        NULL,
        &m_pFrameRoot,
        &m_pAnimeCtrl);
    if(FAILED(hr))
    {
        throw "Xファイルの読み込みに失敗しました";
    }
    // ボーン行列確保
    hr = AllocBone(m_pFrameRoot);
     if(FAILED(hr))
    {
        throw "ボーン行列のメモリ確保に失敗しました";
    }
	 hr = D3DXCreateTextureFromFile(g,filepathTex.c_str(),&pTexture);

}

/**
 * フレーム オブジェクトの割り当てを要求する
 * @param Name        作成するフレームの名前
 * @param *ppNewFrame 作成されたフレーム オブジェクトを返す
 */
HRESULT CAllocateHierarchy::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppRetFrame)
{
    *ppRetFrame = NULL;
    CMeshFrame *pFrame = new CMeshFrame();
    if(pFrame == NULL)
    {
        return E_OUTOFMEMORY;    
    }
    pFrame->Name = new TCHAR[lstrlen(Name) + 1];    
    if(!pFrame->Name)
    {
        return E_FAIL;
    }
    // 各種項目を設定
    strcpy(pFrame->Name, Name);                                // 名前
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);           // トランスフォーム行列
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix); // 最終ワールド行列
    pFrame->pMeshContainer   = NULL; // メッシュコンテナへのポインタ
    pFrame->pFrameSibling    = NULL; // 兄弟フレームへのポインタ
    pFrame->pFrameFirstChild = NULL; // 子フレームへのポインタ
    // 新しいフレームとして設定
    *ppRetFrame = pFrame;
    pFrame = NULL;
    delete pFrame;

    return S_OK;
}

/**
 * メッシュコンテナを作成する
 * @param Name             フレームの名前
 * @param pMeshData        メッシュデータ構造体へのポインタ
 * @param pMaterials       メッシュに使うマテリアルの配列
 * @param pEffectInstances メッシュに使うエフェクトインスタンスの配列
 * @param NumMaterials     マテリアル配列内のマテリアルの数
 * @param *pAdjacency      メッシュの隣接性配列
 * @param pSkinInfo        スキンメッシュへのポインタ
 * @param *ppMeshContainer 作成されたメッシュコンテナを返す
 */
HRESULT CAllocateHierarchy::CreateMeshContainer(
    LPCTSTR Name,
    CONST D3DXMESHDATA *pMeshData,
    CONST D3DXMATERIAL *pMaterials,
    CONST D3DXEFFECTINSTANCE *pEffectInstances,
    DWORD NumMaterials,
    CONST DWORD *pAdjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER *ppMeshContainer) 
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 g = NULL;
    LPD3DXMESH pMesh    = pMeshData->pMesh;
    *ppMeshContainer    = NULL;
    DWORD dwBoneAmt     = 0;

    CMeshContainer *pMeshContainer = new CMeshContainer;
    if(pMeshContainer == NULL)
    {
        return E_OUTOFMEMORY;
    }
    ZeroMemory(pMeshContainer, sizeof(CMeshContainer));

    pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];
    if(!pMeshContainer->Name)
    {
        return E_FAIL;
    }
    // フレーム名設定
    strcpy(pMeshContainer->Name, Name);
    pMesh->GetDevice(&g);
    // メッシュに含まれる面の数を取得する
    INT iFacesAmount = pMesh->GetNumFaces();
    if(!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        // メッシュに法線がないので法線を追加
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
        // 柔軟な頂点フォーマット (FVF) コードを使ってメッシュのコピーを作成する
        hr = pMesh->CloneMeshFVF(
            pMesh->GetOptions(),
            pMesh->GetFVF() | D3DFVF_NORMAL,
            g,
            &pMeshContainer->MeshData.pMesh); // ←ここにコピー
        if(FAILED(hr))
        {
            return E_FAIL;
        }
        pMesh = pMeshContainer->MeshData.pMesh;
        // メッシュに含まれる各頂点の法線を計算して、設定する
        D3DXComputeNormals(pMesh, NULL);
    }
    else 
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type  = D3DXMESHTYPE_MESH;
        // 参照カウンタをインクリメント
        pMesh->AddRef();
    }
    //　メッシュのマテリアル設定
    pMeshContainer->NumMaterials = max(1, NumMaterials);                                 // メッシュに含まれるマテリアルの数
    pMeshContainer->pMaterials   = new D3DXMATERIAL[pMeshContainer->NumMaterials];       // メッシュ マテリアルの配列
    pMeshContainer->ppTextures   = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials]; // テクスチャのポインタへのポインタ
    pMeshContainer->pAdjacency   = new DWORD[iFacesAmount*3];                            // メッシュの三角形ごとに3つのDWORDを持つ、論理隣接性情報が格納された配列へのポインタ。
    if((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        return E_FAIL;
    }
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)*iFacesAmount*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)*pMeshContainer->NumMaterials);

    if(NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
    }
    else
    {
        // マテリアルなし
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular  = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
    if(pSkinInfo != NULL)
    {
        // スキンメッシュあり
        pMeshContainer->pSkinInfo = pSkinInfo;
        // 参照カウンタをインクリメント
        pSkinInfo->AddRef();
        // ボーン数の取得
        dwBoneAmt = pSkinInfo->GetNumBones();
        pMeshContainer->pMatOffset = new D3DXMATRIX[dwBoneAmt];
        for(DWORD i = 0; i < dwBoneAmt; i++)
        {    
            // ボーン行列のコピー
            memcpy(
                &pMeshContainer->pMatOffset[i],
                pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),
                sizeof(D3DMATRIX));    
        }
        // 頂点単位のブレンドの重みとボーンの組み合わせテーブルを適用した新しいメッシュを返す
        if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
            pMesh,                             // 入力メッシュ                       
            NULL,
            pMeshContainer->pAdjacency,        // 入力メッシュの隣接性情報
            NULL, NULL, NULL,
            &pMeshContainer->weight,           // ボーンの重み
            &pMeshContainer->cntBone,          // ボーンの組み合わせテーブルに含まれるボーンの数へのポインタ
            &pMeshContainer->pBoneBuffer,      // ボーンの組み合わせテーブルへのポインタ
            &pMeshContainer->MeshData.pMesh))) // 新しいメッシュへのポインタ
        {
            return E_FAIL;
        }
    }
    //ローカルに生成したメッシュコンテナーを呼び出し側にコピーする
    *ppMeshContainer = pMeshContainer;

    return S_OK;
}

/**
 * ボーン行列のメモリ割り当てと初期化
 * @param pFrame フレーム
 */
HRESULT CAllocateHierarchy::AllocBone(LPD3DXFRAME pFrame)
{
    if(pFrame->pMeshContainer != NULL)
    {
        if(FAILED(AllocBoneMatrix(pFrame->pMeshContainer)))
        {
            return E_FAIL;
        }
    }
    // 兄弟フレームのボーン行列確保
    if(pFrame->pFrameSibling != NULL)
    {
        if(FAILED(AllocBone(pFrame->pFrameSibling)))
        {
            return E_FAIL;
        }
    }
    // 子フレームのボーン行列確保
    if(pFrame->pFrameFirstChild != NULL)
    {
        if(FAILED(AllocBone(pFrame->pFrameFirstChild)))
        {
            return E_FAIL;
        }
    }
    return S_OK;
}

/**
 * ボーン行列のメモリ割り当てと初期化
 */
HRESULT CAllocateHierarchy::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainer)
{
    CMeshContainer *pContainer = (CMeshContainer*)pMeshContainer;
    if (pContainer->pSkinInfo == NULL)
    {
        return S_OK;
    }
    // ボーンの数を取得
    DWORD dwBoneAmt = pContainer->pSkinInfo->GetNumBones();
    // ボーン行列を作る
    pContainer->ppMatRoot = new D3DXMATRIX*[dwBoneAmt];
    for(DWORD i = 0; i < dwBoneAmt; i++)
    {
        // ルートフレームの子フレームを検索
        CMeshFrame *pFrame =
            (CMeshFrame*)D3DXFrameFind(
                m_pFrameRoot,
                pContainer->pSkinInfo->GetBoneName(i));
        if (pFrame == NULL)
        {
            return E_FAIL;
        }
        // ボーン行列に設定
        pContainer->ppMatRoot[i] = &pFrame->CombinedTransformationMatrix;
    }    
    return S_OK;
}

/**
 * フレームを破棄する
 */
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	    if(pFrameToFree->pMeshContainer != NULL)
    {
        if(FAILED(DestroyMeshContainer(pFrameToFree->pMeshContainer)))
        {
            return E_FAIL;
        }
    }
    // 兄弟フレームのボーン行列確保
    if(pFrameToFree->pFrameSibling != NULL)
    {
        if(FAILED(DestroyFrame(pFrameToFree->pFrameSibling)))
        {
            return E_FAIL;
        }
    }
    // 子フレームのボーン行列確保
    if(pFrameToFree->pFrameFirstChild != NULL)
    {
        if(FAILED(DestroyFrame(pFrameToFree->pFrameFirstChild)))
        {
            return E_FAIL;
        }
    }

    SAFE_DELETE_ARRAY(pFrameToFree->Name);
    SAFE_DELETE(pFrameToFree);
    return S_OK; 
}

/**
 * メッシュコンテナを破棄する
 */
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    CMeshContainer *pContainer = (CMeshContainer*)pMeshContainerBase;

    SAFE_DELETE_ARRAY(pContainer->Name);
    SAFE_DELETE_ARRAY(pContainer->pAdjacency);
    SAFE_DELETE_ARRAY(pContainer->pMaterials);

    if (pContainer->ppTextures != NULL)
    {
        for (UINT iMaterial = 0; iMaterial < pContainer->NumMaterials; iMaterial++)
        {
            SAFE_RELEASE(pContainer->ppTextures[iMaterial]);
        }
    }

    SAFE_DELETE_ARRAY(pContainer->ppTextures);
    SAFE_RELEASE(pContainer->MeshData.pMesh);

    SAFE_DELETE(pContainer);
    return S_OK;
}

/**
 * 階層メッシュを描画する
 * @param g 描画デバイス
 */
void CAllocateHierarchy::Draw(LPDIRECT3DDEVICE9 g)
{
    DrawFrame(g, m_pFrameRoot);
}

/**
 * フレームをレンダリングする
 * @param g          描画デバイス
 * @param pFrameBase 現在のフレーム
 */
void CAllocateHierarchy::DrawFrame(LPDIRECT3DDEVICE9 g, LPD3DXFRAME pFrameBase)
{
    // コピー
    CMeshFrame     *pFrame         = (CMeshFrame*)pFrameBase;
    CMeshContainer *pMeshContainer = (CMeshContainer*)pFrame->pMeshContainer;    

    while(pMeshContainer != NULL)
    {
        DrawContainer(g, pMeshContainer, pFrame);
        // 次のメッシュコンテナに進める
        pMeshContainer = (CMeshContainer*)pMeshContainer->pNextMeshContainer;
    }
     // 兄弟フレームを更新
    if(pFrame->pFrameSibling != NULL)
    {
        DrawFrame(g, pFrame->pFrameSibling);
    }
    // 子フレームを更新
    if(pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame(g, pFrame->pFrameFirstChild);
    }
}

/**
 * フレーム内のそれぞれのメッシュをレンダリングする
 * @param g           描画デバイス
 * @param *pContainer メッシュコンテナ
 * @param *pFrame     フレーム
 */
void CAllocateHierarchy::DrawContainer(
    LPDIRECT3DDEVICE9 g,
    CMeshContainer    *pContainer,
    CMeshFrame        *pFrame)
{
    if(pContainer->pSkinInfo != NULL)
    {
        // スキンメッシュあり
        LPD3DXBONECOMBINATION pBoneCombination
            = (LPD3DXBONECOMBINATION)pContainer->pBoneBuffer->GetBufferPointer();
        DWORD dwPrevBoneID = UINT_MAX;
        for(UINT i = 0; i < pContainer->cntBone; i++)
        {
            DWORD dwBlendMatrixAmt = 0;
            for(UINT j = 0; j < pContainer->weight; j++)
            {
                if(pBoneCombination[i].BoneId[j] != UINT_MAX) 
                {
                    dwBlendMatrixAmt = j;
                }
            }
            // ジオメトリブレンディングを実行するために使う行列の個数を設定
            g->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixAmt);
            for(UINT j = 0; j < pContainer->weight; j++) 
            {
                UINT iMatrixIndex = pBoneCombination[i].BoneId[j];
                if(iMatrixIndex != UINT_MAX)
                {
                    // オブセット行列とボーン行列を掛ける
                    D3DXMATRIX matStack =
                        pContainer->pMatOffset[iMatrixIndex]
                        * (*pContainer->ppMatRoot[iMatrixIndex]);
                    // 行列スタックに格納
                    g->SetTransform(D3DTS_WORLDMATRIX(j), &matStack);
                }
            }
            g->SetMaterial(&pContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
            g->SetTexture(0,pTexture);
            dwPrevBoneID = pBoneCombination[i].AttribId;
            pContainer->MeshData.pMesh->DrawSubset(i);    
        }
    }
    else
    {
        // 通常メッシュの場合
        g->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);
        for(UINT i = 0; i < pContainer->NumMaterials; i++)
        {
            g->SetMaterial(&pContainer->pMaterials[i].MatD3D);
            g->SetTexture(0,pTexture);
            pContainer->MeshData.pMesh->DrawSubset(i);
        }
    }
}

/**
 * ワールド座標を設定
 * @param mWorld ワールド座標
 */
void CAllocateHierarchy::SetWorld(LPD3DXMATRIX mWorld)
{
    UpdateWorld(m_pFrameRoot, mWorld);
}

/**
 * フレーム内のメッシュ毎にワールド変換行列を更新
 * @param pFrameBase    現在のフレーム
 * @param pParentMatrix 親行列
 */
void CAllocateHierarchy::UpdateWorld(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
    // コピー
    CMeshFrame *pFrame = (CMeshFrame*)pFrameBase;
    if(pParentMatrix != NULL)
    {
        // 親行列と変換行列を掛け合わせ
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix*(*pParentMatrix);
    }
    else
    {
        // 変換行列のみセット
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
    }
    // 兄弟フレームを更新
    if(pFrame->pFrameSibling != NULL)
    {
        UpdateWorld(
            pFrame->pFrameSibling,
            pParentMatrix);
    }
    // 子フレームを更新
    if(pFrame->pFrameFirstChild != NULL)
    {
        UpdateWorld(
            pFrame->pFrameFirstChild,
            &pFrame->CombinedTransformationMatrix);
    }
}

void CAllocateHierarchy::Release(void)
{
	DestroyFrame(m_pFrameRoot);
	if(m_pAnimeCtrl != NULL)
	{
	m_pAnimeCtrl->Release();
	m_pAnimeCtrl = NULL;
	}
	if(pTexture != NULL)
	{
		pTexture ->Release();
		pTexture = NULL;
	}
}