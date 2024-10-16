#include "pch.h"
#include "CMaterial.h"

// Device Headers
#include "CDevice.h"
#include "CConstantBuffer.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CPathMgr.h"

CMaterial::CMaterial(bool _IsEngine)
	: CAsset(ASSET_TYPE::MATERIAL)
{
	if (_IsEngine)
	{
		SetEngineAsset();
	}
}

CMaterial::~CMaterial()
{
}

void* CMaterial::GetScalarParameter(SCALAR_PARAMETER _Param)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return  m_Const.iArr + _Param;
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return  m_Const.fArr + (_Param - FLOAT_0);
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return  m_Const.v2Arr + (_Param - VEC2_0);
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return  m_Const.v4Arr + (_Param - VEC4_0);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		return  m_Const.matArr + (_Param - MAT_0);
		break;
	}

	return nullptr;
}

void CMaterial::Binding()
{
	if (!m_Shader)
		return;

	for (int i = 0; i < TEXTURE_PARAMETER::END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			m_Const.btex[i] = 0;
			CTexture::Clear(i);
			continue;
		}

		m_Const.btex[i] = 1;
		m_arrTex[i]->Binding(i);
	}

	CConstantBuffer* pCB = CDevice::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding();

	m_Shader->Binding();
}

int CMaterial::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);

	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"wb");

	if (nullptr == File)
		return E_FAIL;

	// Save which shader to reference
	SaveAssetRef(m_Shader, File);

	// Write Constant Values
	fwrite(&m_Const, sizeof(tMtrlConst), 1, File);

	for (UINT i = 0; i < TEXTURE_PARAMETER::END; ++i)
	{
		SaveAssetRef(m_arrTex[i], File);
	}

	fclose(File);

	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	if (nullptr == File)
		return E_FAIL;

	// Load which shader to reference
	LoadAssetRef(m_Shader, File);

	// Read Constant Values
	fread(&m_Const, sizeof(tMtrlConst), 1, File);

	for (UINT i = 0; i < TEXTURE_PARAMETER::END; ++i)
	{
		LoadAssetRef(m_arrTex[i], File);
	}

	fclose(File);

	return S_OK;
}
