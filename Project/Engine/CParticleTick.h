#ifndef CPARTICLETICK_H
#define CPARTICLETICK_H

#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleTick :
    public CComputeShader
{
public:
    CParticleTick();
    ~CParticleTick();

public:
    virtual int Binding() override;
    virtual void CalculateGroupNum() override;
    virtual void Clear() override;

public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnCount(CStructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetModuleBuffer(CStructuredBuffer* _ModuleBuffer) { m_ModuleBuffer = _ModuleBuffer; }

    void SetParticleWorldPosition(Vec3 _Pos) { m_ParticleWorldPos = _Pos; }
    void SetNoiseTexture(Ptr<CTexture> _Noise) { m_NoiseTex = _Noise; }

private:
    CStructuredBuffer*  m_ParticleBuffer;
    CStructuredBuffer*  m_SpawnCountBuffer;
    CStructuredBuffer*  m_ModuleBuffer;

    Ptr<CTexture>       m_NoiseTex;
    Vec3                m_ParticleWorldPos;

};

#endif