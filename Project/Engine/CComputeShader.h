#ifndef CCOMPUTESHADER_H
#define CCOMPUTESHADER_H

#include "CShader.h"

class CComputeShader :
    public CShader
{
public:
    CComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ
                    , const wstring& _ShaderRelativePath, const string& _FuncName);
    ~CComputeShader();

public:
    int Execute();

private:
    int CreateComputeShader(const wstring& _RelativePath, const string& _FuncName);

    virtual int Binding() = 0;
    virtual void CalculateGroupNum() = 0;
    virtual void Clear() = 0;

private:
    ComPtr<ID3DBlob>            m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;

protected:
    // 1 ���� �׷�� ������ ��
    const UINT                  m_ThreadPerGroupX;
    const UINT                  m_ThreadPerGroupY;
    const UINT                  m_ThreadPerGroupZ;

    // �׷� ��
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;

    // ��� ������ ���� �뵵
    tMtrlConst                  m_Const;

};

#endif