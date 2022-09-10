#include "pch.h"
#include "CMapMgr.h"
#include "CLock.h"
#include "CLockGuard.h"
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
    m_lock = new CLock();
}

CMapMgr::~CMapMgr()
{
    delete m_lock;
}

void CMapMgr::Init()
{
    t_MapInfo* map = new t_MapInfo(0,Vector3(-87,0,54),Vector3(228,0,-152),30,2,2,8,3);
    m_maps.Push(0,map);
}

void CMapMgr::End()
{
    
}

t_MapInfo* CMapMgr::GetMapInfo(UINT _mapid)
{
    CLockGuard<CLock> lock(m_lock);
    t_MapInfo* dummy = nullptr;
    if (m_maps.Find(_mapid,dummy)==false)
    {
        return nullptr;
    }
    else
    {
        return m_maps[_mapid];
    }
}

