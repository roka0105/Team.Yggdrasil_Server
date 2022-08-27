#include "pch.h"
#include "CGameMgr.h"
#include "CMapMgr.h"
#include "CSectorMgr.h"

CGameMgr* CGameMgr::m_instance = nullptr;

void CGameMgr::Create()
{
    if (m_instance == nullptr)
    {
        m_instance = new CGameMgr();
    }
    CMapMgr::Create();
    CSectorMgr::Create();
}

void CGameMgr::Destroy()
{
    CMapMgr::Destroy();
    CSectorMgr::Destory();
    delete m_instance;
}

CGameMgr* CGameMgr::GetInst()
{
    return m_instance;
}

CGameMgr::CGameMgr()
{

}

CGameMgr::~CGameMgr()
{
}

void CGameMgr::Init()
{
    CMapMgr::GetInst()->Init();
    CSectorMgr::GetInst()->Init();
}

void CGameMgr::End()
{
    CMapMgr::GetInst()->End();
    CSectorMgr::GetInst()->End();
}

void CGameMgr::GameProcess(CSession* _session)
{

}

void CGameMgr::LoadingProcess(CSession* _session)
{
    CSectorMgr::GetInst()->SetViewNode(_session);
    CSectorMgr::GetInst()->SendInit(_session);
}


