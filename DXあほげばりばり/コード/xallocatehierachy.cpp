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
 * ���b�V���ǂݍ���
 * @param g        �`��f�o�C�X
 * @param filepath �t�@�C���p�X
 */
void CAllocateHierarchy::Load(LPDIRECT3DDEVICE9 g, string filepath,string filepathTex)
{
    // ���b�V���ǂݍ���
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
        throw "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���";
    }
    // �{�[���s��m��
    hr = AllocBone(m_pFrameRoot);
     if(FAILED(hr))
    {
        throw "�{�[���s��̃������m�ۂɎ��s���܂���";
    }
	 hr = D3DXCreateTextureFromFile(g,filepathTex.c_str(),&pTexture);

}

/**
 * �t���[�� �I�u�W�F�N�g�̊��蓖�Ă�v������
 * @param Name        �쐬����t���[���̖��O
 * @param *ppNewFrame �쐬���ꂽ�t���[�� �I�u�W�F�N�g��Ԃ�
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
    // �e�퍀�ڂ�ݒ�
    strcpy(pFrame->Name, Name);                                // ���O
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);           // �g�����X�t�H�[���s��
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix); // �ŏI���[���h�s��
    pFrame->pMeshContainer   = NULL; // ���b�V���R���e�i�ւ̃|�C���^
    pFrame->pFrameSibling    = NULL; // �Z��t���[���ւ̃|�C���^
    pFrame->pFrameFirstChild = NULL; // �q�t���[���ւ̃|�C���^
    // �V�����t���[���Ƃ��Đݒ�
    *ppRetFrame = pFrame;
    pFrame = NULL;
    delete pFrame;

    return S_OK;
}

/**
 * ���b�V���R���e�i���쐬����
 * @param Name             �t���[���̖��O
 * @param pMeshData        ���b�V���f�[�^�\���̂ւ̃|�C���^
 * @param pMaterials       ���b�V���Ɏg���}�e���A���̔z��
 * @param pEffectInstances ���b�V���Ɏg���G�t�F�N�g�C���X�^���X�̔z��
 * @param NumMaterials     �}�e���A���z����̃}�e���A���̐�
 * @param *pAdjacency      ���b�V���̗אڐ��z��
 * @param pSkinInfo        �X�L�����b�V���ւ̃|�C���^
 * @param *ppMeshContainer �쐬���ꂽ���b�V���R���e�i��Ԃ�
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
    // �t���[�����ݒ�
    strcpy(pMeshContainer->Name, Name);
    pMesh->GetDevice(&g);
    // ���b�V���Ɋ܂܂��ʂ̐����擾����
    INT iFacesAmount = pMesh->GetNumFaces();
    if(!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        // ���b�V���ɖ@�����Ȃ��̂Ŗ@����ǉ�
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
        // �_��Ȓ��_�t�H�[�}�b�g (FVF) �R�[�h���g���ă��b�V���̃R�s�[���쐬����
        hr = pMesh->CloneMeshFVF(
            pMesh->GetOptions(),
            pMesh->GetFVF() | D3DFVF_NORMAL,
            g,
            &pMeshContainer->MeshData.pMesh); // �������ɃR�s�[
        if(FAILED(hr))
        {
            return E_FAIL;
        }
        pMesh = pMeshContainer->MeshData.pMesh;
        // ���b�V���Ɋ܂܂��e���_�̖@�����v�Z���āA�ݒ肷��
        D3DXComputeNormals(pMesh, NULL);
    }
    else 
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type  = D3DXMESHTYPE_MESH;
        // �Q�ƃJ�E���^���C���N�������g
        pMesh->AddRef();
    }
    //�@���b�V���̃}�e���A���ݒ�
    pMeshContainer->NumMaterials = max(1, NumMaterials);                                 // ���b�V���Ɋ܂܂��}�e���A���̐�
    pMeshContainer->pMaterials   = new D3DXMATERIAL[pMeshContainer->NumMaterials];       // ���b�V�� �}�e���A���̔z��
    pMeshContainer->ppTextures   = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials]; // �e�N�X�`���̃|�C���^�ւ̃|�C���^
    pMeshContainer->pAdjacency   = new DWORD[iFacesAmount*3];                            // ���b�V���̎O�p�`���Ƃ�3��DWORD�����A�_���אڐ���񂪊i�[���ꂽ�z��ւ̃|�C���^�B
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
        // �}�e���A���Ȃ�
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular  = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
    if(pSkinInfo != NULL)
    {
        // �X�L�����b�V������
        pMeshContainer->pSkinInfo = pSkinInfo;
        // �Q�ƃJ�E���^���C���N�������g
        pSkinInfo->AddRef();
        // �{�[�����̎擾
        dwBoneAmt = pSkinInfo->GetNumBones();
        pMeshContainer->pMatOffset = new D3DXMATRIX[dwBoneAmt];
        for(DWORD i = 0; i < dwBoneAmt; i++)
        {    
            // �{�[���s��̃R�s�[
            memcpy(
                &pMeshContainer->pMatOffset[i],
                pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),
                sizeof(D3DMATRIX));    
        }
        // ���_�P�ʂ̃u�����h�̏d�݂ƃ{�[���̑g�ݍ��킹�e�[�u����K�p�����V�������b�V����Ԃ�
        if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
            pMesh,                             // ���̓��b�V��                       
            NULL,
            pMeshContainer->pAdjacency,        // ���̓��b�V���̗אڐ����
            NULL, NULL, NULL,
            &pMeshContainer->weight,           // �{�[���̏d��
            &pMeshContainer->cntBone,          // �{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[���̐��ւ̃|�C���^
            &pMeshContainer->pBoneBuffer,      // �{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
            &pMeshContainer->MeshData.pMesh))) // �V�������b�V���ւ̃|�C���^
        {
            return E_FAIL;
        }
    }
    //���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[����
    *ppMeshContainer = pMeshContainer;

    return S_OK;
}

/**
 * �{�[���s��̃��������蓖�ĂƏ�����
 * @param pFrame �t���[��
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
    // �Z��t���[���̃{�[���s��m��
    if(pFrame->pFrameSibling != NULL)
    {
        if(FAILED(AllocBone(pFrame->pFrameSibling)))
        {
            return E_FAIL;
        }
    }
    // �q�t���[���̃{�[���s��m��
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
 * �{�[���s��̃��������蓖�ĂƏ�����
 */
HRESULT CAllocateHierarchy::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainer)
{
    CMeshContainer *pContainer = (CMeshContainer*)pMeshContainer;
    if (pContainer->pSkinInfo == NULL)
    {
        return S_OK;
    }
    // �{�[���̐����擾
    DWORD dwBoneAmt = pContainer->pSkinInfo->GetNumBones();
    // �{�[���s������
    pContainer->ppMatRoot = new D3DXMATRIX*[dwBoneAmt];
    for(DWORD i = 0; i < dwBoneAmt; i++)
    {
        // ���[�g�t���[���̎q�t���[��������
        CMeshFrame *pFrame =
            (CMeshFrame*)D3DXFrameFind(
                m_pFrameRoot,
                pContainer->pSkinInfo->GetBoneName(i));
        if (pFrame == NULL)
        {
            return E_FAIL;
        }
        // �{�[���s��ɐݒ�
        pContainer->ppMatRoot[i] = &pFrame->CombinedTransformationMatrix;
    }    
    return S_OK;
}

/**
 * �t���[����j������
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
    // �Z��t���[���̃{�[���s��m��
    if(pFrameToFree->pFrameSibling != NULL)
    {
        if(FAILED(DestroyFrame(pFrameToFree->pFrameSibling)))
        {
            return E_FAIL;
        }
    }
    // �q�t���[���̃{�[���s��m��
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
 * ���b�V���R���e�i��j������
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
 * �K�w���b�V����`�悷��
 * @param g �`��f�o�C�X
 */
void CAllocateHierarchy::Draw(LPDIRECT3DDEVICE9 g)
{
    DrawFrame(g, m_pFrameRoot);
}

/**
 * �t���[���������_�����O����
 * @param g          �`��f�o�C�X
 * @param pFrameBase ���݂̃t���[��
 */
void CAllocateHierarchy::DrawFrame(LPDIRECT3DDEVICE9 g, LPD3DXFRAME pFrameBase)
{
    // �R�s�[
    CMeshFrame     *pFrame         = (CMeshFrame*)pFrameBase;
    CMeshContainer *pMeshContainer = (CMeshContainer*)pFrame->pMeshContainer;    

    while(pMeshContainer != NULL)
    {
        DrawContainer(g, pMeshContainer, pFrame);
        // ���̃��b�V���R���e�i�ɐi�߂�
        pMeshContainer = (CMeshContainer*)pMeshContainer->pNextMeshContainer;
    }
     // �Z��t���[�����X�V
    if(pFrame->pFrameSibling != NULL)
    {
        DrawFrame(g, pFrame->pFrameSibling);
    }
    // �q�t���[�����X�V
    if(pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame(g, pFrame->pFrameFirstChild);
    }
}

/**
 * �t���[�����̂��ꂼ��̃��b�V���������_�����O����
 * @param g           �`��f�o�C�X
 * @param *pContainer ���b�V���R���e�i
 * @param *pFrame     �t���[��
 */
void CAllocateHierarchy::DrawContainer(
    LPDIRECT3DDEVICE9 g,
    CMeshContainer    *pContainer,
    CMeshFrame        *pFrame)
{
    if(pContainer->pSkinInfo != NULL)
    {
        // �X�L�����b�V������
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
            // �W�I���g���u�����f�B���O�����s���邽�߂Ɏg���s��̌���ݒ�
            g->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixAmt);
            for(UINT j = 0; j < pContainer->weight; j++) 
            {
                UINT iMatrixIndex = pBoneCombination[i].BoneId[j];
                if(iMatrixIndex != UINT_MAX)
                {
                    // �I�u�Z�b�g�s��ƃ{�[���s����|����
                    D3DXMATRIX matStack =
                        pContainer->pMatOffset[iMatrixIndex]
                        * (*pContainer->ppMatRoot[iMatrixIndex]);
                    // �s��X�^�b�N�Ɋi�[
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
        // �ʏ탁�b�V���̏ꍇ
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
 * ���[���h���W��ݒ�
 * @param mWorld ���[���h���W
 */
void CAllocateHierarchy::SetWorld(LPD3DXMATRIX mWorld)
{
    UpdateWorld(m_pFrameRoot, mWorld);
}

/**
 * �t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V
 * @param pFrameBase    ���݂̃t���[��
 * @param pParentMatrix �e�s��
 */
void CAllocateHierarchy::UpdateWorld(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
    // �R�s�[
    CMeshFrame *pFrame = (CMeshFrame*)pFrameBase;
    if(pParentMatrix != NULL)
    {
        // �e�s��ƕϊ��s����|�����킹
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix*(*pParentMatrix);
    }
    else
    {
        // �ϊ��s��̂݃Z�b�g
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
    }
    // �Z��t���[�����X�V
    if(pFrame->pFrameSibling != NULL)
    {
        UpdateWorld(
            pFrame->pFrameSibling,
            pParentMatrix);
    }
    // �q�t���[�����X�V
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