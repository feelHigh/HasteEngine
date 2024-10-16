#pragma once
#include "CEntity.h"

class CConstantBuffer :
    public CEntity
{
public:
    CLONE_DISABLE(CConstantBuffer);
    CConstantBuffer();
    ~CConstantBuffer();

public:
    int Create(CONSTANT_BUFFER_TYPE _type, UINT _BufferSize);

public:
    void SetData(void* _pData);
    void Binding();
    void Binding_CS();

private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;
    CONSTANT_BUFFER_TYPE    m_CBType;

};
