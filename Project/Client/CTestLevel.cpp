#include "pch.h"
#include "CTestLevel.h"

#include <Engine/assets.h>
#include <Engine/components.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include <Engine/CGameObject.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CSetColorCS.h>
#include <Engine/CStructuredBuffer.h>

#include <Engine/CLevelSaveLoad.h>

void CTestLevel::CreateTestLevel()
{
	// Material
	Ptr<CMaterial> pStd3DMtrl = CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"Std3DMtrl");
	Ptr<CMaterial> pStd3D_DeferredMtrl = CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"Std3D_DeferredMtrl");

	// Level
	CLevel* pLevel = new CLevel;

	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"Monster");
	pLevel->GetLayer(5)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(6)->SetName(L"MonsterProjectile");

	// Camera
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	CamObj->AddComponent(new CCameraMoveScript);

	// 우선순위를 0 : MainCamera 로 설정
	CamObj->Camera()->SetPriority(0);

	// 카메라 레이어 설정 (31번 레이어 제외 모든 레이어를 촬영)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(100000.f);
	CamObj->Camera()->SetProjectionType(PERSPECTIVE);

	pLevel->AddObject(0, CamObj);

	CGameObject* pObject = nullptr;

	// 3D Light
	pObject = new CGameObject;
	pObject->SetName(L"Directinal Light");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight3D);

	pObject->Transform()->SetRelativePosition(-300.f, 0.f, 0.f);
	pObject->Transform()->SetRelativeRotation(XM_PI / 4.f, XM_PI / 4.f, 0.f);

	pObject->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light3D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
	pObject->Light3D()->SetLightAmbient(Vec3(0.1f, 0.1f, 0.1f));
	pObject->Light3D()->SetSpecularCoefficient(0.3f);
	pObject->Light3D()->SetRadius(800.f);

	pLevel->AddObject(0, pObject);


	// SkyBox
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkybox);

	pSkyBox->Transform()->SetRelativePosition(0.f, 0.f, 0.f);
	pSkyBox->Transform()->SetRelativeScale(1000.f, 1000.f, 1000.f);

	//Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"Texture\\Skybox\\SkyWater.dds");
	//pSkyBox->SkyBox()->SetSkyBoxTexture(pSkyBoxTex);
	//pSkyBox->SkyBox()->SetSkyBoxType(CUBE);		

	Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"Texture\\Skybox\\SkyDawn.dds");
	pSkyBox->Skybox()->SetSkyboxTexture(pSkyBoxTex);
	pSkyBox->Skybox()->SetSkyboxType(CUBE);

	pLevel->AddObject(0, pSkyBox);

	// Player
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);

	pPlayer->Transform()->SetRelativePosition(0.f, -500.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(1000.f, 1000.f, 1.f);
	pPlayer->Transform()->SetRelativeRotation(XM_PI / 2.f, 0.f, 0.f);

	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->MeshRender()->SetMaterial(pStd3D_DeferredMtrl);

	Ptr<CTexture> pTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"Texture\\LandScapeTexture\\gl1_ground_II_albedo.TGA");
	Ptr<CTexture> pNTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"Texture\\LandScapeTexture\\gl1_ground_II_normal.TGA");

	pPlayer->MeshRender()->GetMaterial()->SetTextureParameter(TEX_0, pTex);		// Albedo Parameter
	//pPlayer->MeshRender()->GetMaterial()->SetTextureParameter(TEX_1, pNTex);	// Normal Parameter

	pLevel->AddObject(3, pPlayer);

	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	// 충돌 지정
	CCollisionMgr::GetInstance()->CollisionCheck(3, 4); // Player vs Monster
	CCollisionMgr::GetInstance()->CollisionCheck(5, 4); // Player Projectile vs Monster	
}

void CTestLevel::CreatePrefab()
{
	/*CGameObject* pProto = new CGameObject;
	pProto->SetName("Missile");
	pProto->AddComponent(new CTransform);
	pProto->AddComponent(new CMeshRender);
	pProto->AddComponent(new CMissileScript);

	pProto->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pProto->MeshRender()->SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"RectMesh"));
	pProto->MeshRender()->SetMaterial(CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pProto);

	CAssetMgr::GetInstance()->AddAsset<CPrefab>(L"MissilePref", pPrefab);

	wstring FilePath = CPathMgr::GetInstance()->GetContentPath();
	FilePath += L"Prefab\\Missile.pref";
	pPrefab->Save(FilePath);*/
}

