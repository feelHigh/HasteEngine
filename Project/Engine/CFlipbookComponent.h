#ifndef CFLIPBOOKCOMPONENT_H
#define CFLIPBOOKCOMPONENT_H

#include "CComponent.h"

#include "CFlipbook.h"

class CFlipbookComponent :
    public CComponent
{
public:
    CLONE(CFlipbookComponent);
    CFlipbookComponent();
    CFlipbookComponent(CFlipbookComponent& _Origin);
    ~CFlipbookComponent();

public:
    virtual void FinalTick() override;

public:
    void AddFlipbook(int _Idx, Ptr<CFlipbook> _Flipbook);
    Ptr<CFlipbook> FindFlipbook(const wstring& _Key);
    void Play(int _FliBookIdx, float _FPS, bool _Repeat);
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    void Reset();
    void Binding();
    static void Clear();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    vector<Ptr<CFlipbook>>  m_vecFlipbook;  // FliBook 컴포넌트가 보유한 모든 Flipbook 목록
    Ptr<CFlipbook>          m_CurFlipbook;  // 현재 재생중인 Flipbook
    Ptr<CSprite>            m_CurFrmSprite; // 현재 재생중인 Flipbook 에서 현재 프레임 인덱스에 해당하는 스프라이트
    int                     m_CurFrmIdx;    // 현재 재생중인 Flipbook 에서 몇번째 Sprite 가 재생중인지 인덱스 기록
    float                   m_FPS;          // 현재 재생중인 Flipbook 의 초당 프레임 진행 수
    float                   m_AccTime;      // 누적 시간값 체크
    bool                    m_Repeat;
    bool                    m_Finish;       // Flipbook 재생이 끝에 도달했는지 여부

};

#endif