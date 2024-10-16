#ifndef CSHADER_H
#define CSHADER_H

#include "CAsset.h"

class CShader :
    public CAsset
{
public:
    CShader(ASSET_TYPE _Type);
    ~CShader();

public:
    virtual int Load(const wstring& _FilePath) { return S_OK; }
    virtual int Save(const wstring& _FilePath) { return S_OK; }

protected:
    ComPtr<ID3DBlob> m_ErrBlob;

};

#endif