#pragma once
#include "CAsset.h"

#include "Ptr.h"
#include "CGraphicShader.h"
#include "CTexture.h"

class CMaterial :
    public CAsset
{
public:
    CLONE(CMaterial);
    CMaterial(bool _IsEngine = false);
    ~CMaterial();

public:
    template<typename T>
    void SetScalarParameter(SCALAR_PARAMETER _Param, const T& _Data);
    void SetTextureParameter(TEXTURE_PARAMETER _Param, Ptr<CTexture> _Tex) { m_arrTex[_Param] = _Tex; }

    void* GetScalarParameter(SCALAR_PARAMETER _Param);
    Ptr<CTexture> GetTextureParameter(TEXTURE_PARAMETER _Param) { return m_arrTex[(UINT)_Param]; }

public:
    void SetShader(Ptr<CGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<CGraphicShader> GetShader() { return m_Shader; }

public:
    void Binding();

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _RelativePath) override;

private:
    Ptr<CGraphicShader>     m_Shader;

    tMtrlConst              m_Const;
    Ptr<CTexture>           m_arrTex[TEXTURE_PARAMETER::END];
};

template<typename T>
void CMaterial::SetScalarParameter(SCALAR_PARAMETER _Param, const T& _Data)
{
    if constexpr (std::is_same_v<T, int>) { m_Const.iArr[_Param] = _Data; return; }
    if constexpr (std::is_same_v<T, float>) { m_Const.fArr[_Param - FLOAT_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Vec2>) { m_Const.v2Arr[_Param - VEC2_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Vec4>) { m_Const.v4Arr[_Param - VEC4_0] = _Data; return; }
    if constexpr (std::is_same_v<T, Matrix>) { m_Const.matArr[_Param - MAT_0] = _Data; return; }

    assert(nullptr);
}
