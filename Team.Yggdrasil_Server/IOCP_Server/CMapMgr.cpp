#include "CMapMgr.h"
#include "pch.h"
#include "CMapMgr.h"
CMapMgr* CMapMgr::m_instance = nullptr;
CMapMgr* CMapMgr::GetInst()
{
    return m_instance;
}

void CMapMgr::Create()
{
    if (m_instance == nullptr)
    {
        m_instance = new CMapMgr();
    }
}

void CMapMgr::Destroy()
{
    delete m_instance;
}

CMapMgr::CMapMgr()
{
}

CMapMgr::~CMapMgr()
{
}

void CMapMgr::Init()
{
}

void CMapMgr::End()
{
    
}
