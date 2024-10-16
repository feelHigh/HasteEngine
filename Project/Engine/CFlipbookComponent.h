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
    vector<Ptr<CFlipbook>>  m_vecFlipbook;  // FliBook ������Ʈ�� ������ ��� Flipbook ���
    Ptr<CFlipbook>          m_CurFlipbook;  // ���� ������� Flipbook
    Ptr<CSprite>            m_CurFrmSprite; // ���� ������� Flipbook ���� ���� ������ �ε����� �ش��ϴ� ��������Ʈ
    int                     m_CurFrmIdx;    // ���� ������� Flipbook ���� ���° Sprite �� ��������� �ε��� ���
    float                   m_FPS;          // ���� ������� Flipbook �� �ʴ� ������ ���� ��
    float                   m_AccTime;      // ���� �ð��� üũ
    bool                    m_Repeat;
    bool                    m_Finish;       // Flipbook ����� ���� �����ߴ��� ����

};

#endif