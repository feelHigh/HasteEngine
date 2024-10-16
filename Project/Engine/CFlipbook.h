#pragma once
#include "CAsset.h"

#include "CSprite.h"

class CFlipbook :
    public CAsset
{
public:
    CFlipbook(bool _IsEngine = false);
    ~CFlipbook();

public:
    void FinalTick();

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }
    int GetMaxFrameCount() { return (int)m_vecSprite.size(); }

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override;

private:
    vector<Ptr<CSprite>>    m_vecSprite;

};