#include "pch.h"
#include "CLobbyMgr.h"
#include "CLock.h"
#include "CLockGuard.h"

CLobbyMgr* CLobbyMgr::instance = nullptr;
CLobbyMgr::CLobbyMgr()
{
    m_lock = new CLock();
}
CLobbyMgr::~CLobbyMgr()
{
    delete m_lock;
}

CLobbyMgr* CLobbyMgr::GetInst()
{
    return instance;
}

void CLobbyMgr::Create()
{
    if (instance == nullptr)
        instance = new CLobbyMgr();
}

void CLobbyMgr::Destroy()
{
    delete instance;
}

void CLobbyMgr::Init()
{

}

void CLobbyMgr::End()
{
}

void CLobbyMgr::MultiModeProcess()
{
}

void CLobbyMgr::SingleModeProcess()
{
}
