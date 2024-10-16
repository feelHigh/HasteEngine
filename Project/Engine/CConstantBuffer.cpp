#include "pch.h"
#include "CConstantBuffer.h"

// Device Headers
#include "CDevice.h"

CConstantBuffer::CConstantBuffer()
    : m_Desc{}
{
}

CConstantBuffer::~CConstantBuffer()
{
}

int CConstantBuffer::Create(CONSTANT_BUFFER_TYPE _type, UINT _BufferSize)
{
    assert(0 == (_BufferSize % 16));

    m_CBType = _type;
    m_Desc.ByteWidth = _BufferSize;
    m_Desc.Usage = D3D11_USAGE_DYNAMIC;
    m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
    {
        return E_FAIL;
    }
    return S_OK;
}

void CConstantBuffer::SetData(void* _pData)
{
    D3D11_MAPPED_SUBRESOURCE tMapSub = {};
    CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
    memcpy(tMapSub.pData, _pData, m_Desc.ByteWidth);
    CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstantBuffer::Binding()
{
    CONTEXT->VSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
    CONTEXT->HSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
    CONTEXT->DSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
    CONTEXT->GSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
    CONTEXT->PSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
    CONTEXT->CSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
}

void CConstantBuffer::Binding_CS()
{
    CONTEXT->CSSetConstantBuffers((UINT)m_CBType, 1, m_CB.GetAddressOf());
}
