#pragma once
#include "CRenderComponent.h"

#include "CParticleTick.h"

class CStructuredBuffer;

struct tSpawnCount
{
    UINT    SpawnCount;
    UINT    padding[3];
};

class CParticleSystem :
    public CRenderComponent
{
public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _Other);
    ~CParticleSystem();

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    void CaculateSpawnCount();
    void SetParticleTexture(Ptr<CTexture> _Texture) { m_ParticleTex = _Texture; }

public:
    virtual void SaveToFile(FILE* _File) override;   // ���Ͽ� ������Ʈ�� ������ ����
    virtual void LoadFromFile(FILE* _File) override; // ���Ͽ� ������Ʈ�� ������ �ε�

private:
    Ptr<CParticleTick>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;       // ��� ��ƼŬ ����
    CStructuredBuffer*      m_SpawnCountBuffer;     // ��ƼŬ Ȱ��ȭ ���� ���޿� ����
    CStructuredBuffer*      m_ModuleBuffer;         // Module Data Buffer

    Ptr<CTexture>           m_ParticleTex;
    float                   m_Time;                 // �����ð�    
    float                   m_BurstTime;            // SpawnBurst üũ�� ����

    int                     m_MaxParticeCount;      // ��ƼŬ �ִ� ����

    tParticleModule         m_Module;               // ��ƼŬ�� ��� ����

};