#ifndef _FUNC
#define _FUNC

#include "Global.hlsli"
#include "Struct.hlsli"

int IsBinding(Texture2D _tex)
{
    int width = 0, height = 0;
    _tex.GetDimensions(width, height);
    
    if (0 == width || height == 0)
        return false;
    
    return true;
}

void CalculateLight2D(int _LightIdx, float3 _WorldPos, inout tLight _Light)
{
    tLightInfo Info = g_Light2D[_LightIdx];
    
    // DirectionalLight �� ���
    if (0 == Info.Type)
    {
        //_Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.LightEssence.Ambient.rgb;
    }
    
    // PointLight �� ���
    else if (1 == Info.Type)
    {
         // �������� �ȼ������� �Ÿ�
        /*
        float fDist = distance(Info.WorldPosition.xy, _WorldPosition.xy);
        
        // �Ÿ����� ������ ġȯ�ؼ� �Ÿ��� ���� ���� ���⸦ �ڻ��� �׷��� ���·� ����Ѵ�.
        float fPow = saturate(cos((fDist / Info.Radius) * (PI / 2.f)));
        
        // �������κ��� ������ �Ÿ��� ���� ���� ����
        //float fPow2 = saturate(1.f - fDist / Info.Radius);
        
        // ���� ���� ��� = ���� �� * �Ÿ������� ����
        _Light.Color.rgb += Info.LightInfo.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.LightInfo.Ambient.rgb;
        */
        float fAttenu = 1.f;
        
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);
        if (fDist < Info.Radius)
        {
            if (g_int_0)
            {
                float fTheta = (fDist / Info.Radius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            }
            else
            {
                fAttenu = saturate(1.f - fDist / g_Light2D[0].Radius);
            }
            
            _Light.Color += Info.LightEssence.Color * fAttenu;
        }
    }
    
    // SpotLight �� ���
    else
    {
        float fAttenu = 1.f;

        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);
        float2 toPoint = normalize(Info.WorldPos.xy - _WorldPos.xy); // ������ġ-> ������ġ�� ���ϴ� ���⺤��
        float cosTheta = dot(normalize(Info.WorldDir.xy), toPoint); // ���� ���ϴ� ����� ���� �޴� �������� ���Ͱ��� ����
        
        if (fDist < Info.Radius)
        {
            // cosTheta > info.fAngle / 2.f : ���� �ȼ��� �߻��ϴ� ���� ���� ���� ���� ����
            if (cosTheta > cos(radians(Info.Angle) / 2.f))
            {
                float fTheta = (fDist / Info.Radius) * (PI / 2.f);
                fAttenu = saturate(cos(fTheta));
            
                _Light.Color += Info.LightEssence.Color * fAttenu;
            }
        }
    }
}


void CalculateLight3D(int _LightIdx, float3 _ViewNormal, float3 _ViewPos, inout tLight _Light)
{
    tLightInfo LightInfo = g_Light3D[_LightIdx];
    
    float LightPow = 0.f;
    float SpecularPow = 0.f;
    float Ratio = 1.f;
    float SpecRatio = 1.f;
    
    // Directional Light
    if (0 == LightInfo.Type)
    {
        // VS ���� ���� �븻������, ���� ���⸦ PS ���� ���� ���Ѵ��� ���� ���⸦ ����
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // �ݻ籤 ���
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
            
        // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
        // �ȼ��� �佺���̽� ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
        float3 vEye = normalize(_ViewPos);
    
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    }
    
    // Point Light
    else if (1 == LightInfo.Type)
    {
        // ǥ�� ��ġ���� ������ ��ġ�� ����. �������� ǥ���� ���ϴ� ���⺤�͸� ���� �� �ִ�.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // �ݻ籤 ���
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
            
        // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
        // �ȼ��� �佺���̽� ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
        float3 vEye = normalize(_ViewPos);
    
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
             
        // �Ÿ��� ���� ���� ���� ���ҷ��� ����Ѵ�.
        float Distance = length(vLightViewPos - _ViewPos);
        float CamDistance = length(_ViewPos);
        //Ratio = saturate(1.f - (Distance / LightInfo.Radius));
        Ratio = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = saturate(cos((PI / 2.f) * saturate(CamDistance / LightInfo.Radius)));
    }
    
    // Spot Light
    else if (2 == LightInfo.Type)
    {
        
    }
      
    // ǥ���� �޴� �� = ������ �� * ǥ���� �޴� ���� ���� * �Ÿ��� ���� ���� ���ҷ�
    _Light.Color += LightInfo.LightEssence.Color * LightPow * Ratio;
    _Light.Ambient += LightInfo.LightEssence.Ambient * Ratio;
    _Light.SpecCoef += LightInfo.LightEssence.SpecCoef * SpecularPow * SpecRatio;
}

float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_EngineTime * 0.5f;
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_1, vUV, 0).xyz;
    
    return vRandom;
}

#endif