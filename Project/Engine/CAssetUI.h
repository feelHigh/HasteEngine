#pragma once
#include "CEditor.h"

class CAssetUI :
    public CEditor
{
public:
    CAssetUI(ASSET_TYPE _Type);
    ~CAssetUI();

public:
    void SetAsset(Ptr<CAsset> _Asset);
    Ptr<CAsset> GetAsset() { return m_Asset; }

private:
    Ptr<CAsset>         m_Asset;
    const ASSET_TYPE    m_Type;

protected:
    void Title();
    void OutputAssetName();

};
