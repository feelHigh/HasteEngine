#include "pch.h"
#include "CComputeShader.h"

// Device Headers
#include "CDevice.h"
#include "CConstantBuffer.h"

// Manager Headers
#include "CPathMgr.h"

CComputeShader::CComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ
	, const wstring& _ShaderRelativePath, const string& _FuncName)
	: CShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_ThreadPerGroupX(_ThreadPerGroupX)
	, m_ThreadPerGroupY(_ThreadPerGroupY)
	, m_ThreadPerGroupZ(_ThreadPerGroupZ)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_Const{}
{
	CreateComputeShader(_ShaderRelativePath, _FuncName);
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::Execute()
{
	if (FAILED(Binding()))
		return E_FAIL;

	// ��������� ����
	CConstantBuffer* pCB = CDevice::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding_CS();

	// �ʿ��� �׷� �� ���
	CalculateGroupNum();

	// ��ǻƮ ���̴� ����
	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	// ���ҽ� ����
	Clear();
}

int CComputeShader::CreateComputeShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _RelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
									, _FuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0
									, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Compute Shader compile failed.", MB_OK);
		}
		else
		{
			errno_t err = GetLastError();
			wchar_t szErrMsg[255] = {};
			swprintf_s(szErrMsg, 255, L"Error Code : %d", err);
			MessageBox(nullptr, szErrMsg, L"Compute Shader compile failed.", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
								, m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf());

	return S_OK;
}