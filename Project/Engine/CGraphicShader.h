#pragma once
#include "CShader.h"

struct tScalarParam
{
    SCALAR_PARAMETER    ParamType;
    string              strDesc;
};

struct tTexParam
{
    TEXTURE_PARAMETER   ParamType;
    string              SstrDesc;
};


class CGraphicShader :
    public CShader
{
public:
    CGraphicShader();
    ~CGraphicShader();

public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);

    void Binding();
public:
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }

    void SetDomain(SHADER_DOMAIN _Domain) { m_Domain = _Domain; }
    void SetRasterizerStateType(RASTERIZER_STATE_TYPE _Type) { m_RSType = _Type; }
    void SetDepthStencilStateType(DEPTH_STENCIL_STATE_TYPE _Type) { m_DSType = _Type; }
    void SetBlendStateType(BLEND_STATE_TYPE _Type) { m_BSType = _Type; }

    SHADER_DOMAIN GetDomain() { return m_Domain; }
    RASTERIZER_STATE_TYPE GetRasterizerStateType() { return m_RSType; }
    DEPTH_STENCIL_STATE_TYPE GetDepthStencilStateType() { return m_DSType; }
    BLEND_STATE_TYPE GetBlendStateType() { return m_BSType; }

    void AddScalarParam(SCALAR_PARAMETER _Type, const string& _Desc) { m_ScalarParam.push_back(tScalarParam{ _Type, _Desc }); }
    void AddTexParam(TEXTURE_PARAMETER _Type, const string& _Desc) { m_TexParam.push_back(tTexParam{ _Type, _Desc }); }

    const vector<tScalarParam>& GetScalarParam() { return m_ScalarParam; }
    const vector<tTexParam>& GetTexParam() { return m_TexParam; }

private:
    ComPtr<ID3DBlob>			    m_VSBlob;
    ComPtr<ID3DBlob>			    m_GSBlob;
    ComPtr<ID3DBlob>			    m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>	    m_PS;

    ComPtr<ID3D11InputLayout>       m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_Topology;

    RASTERIZER_STATE_TYPE           m_RSType;
    DEPTH_STENCIL_STATE_TYPE        m_DSType;
    BLEND_STATE_TYPE                m_BSType;

    SHADER_DOMAIN                   m_Domain;   // ½¦ÀÌ´õÀÇ ·»´õ¸µ ¹æ½Ä

    // Shader Parameter
    vector<tScalarParam>            m_ScalarParam;
    vector<tTexParam>               m_TexParam;

};
