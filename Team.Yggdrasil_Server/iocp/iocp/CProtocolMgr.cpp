#include "pch.h"
#include "CProtocolMgr.h"
CProtocolMgr* CProtocolMgr::instance = nullptr;

CProtocolMgr* CProtocolMgr::GetInst()
{
	return instance;
}

void CProtocolMgr::Create()
{
	if (instance == nullptr)
	{
		instance = new CProtocolMgr();
	}
}

void CProtocolMgr::Destroy()
{
	delete instance;
}

unsigned long CProtocolMgr::GetMainProtocol(unsigned long _protocolmem)
{
	unsigned long temp_protocol;
	temp_protocol = (_protocolmem & or_mainbin) >> mainshift;
	return temp_protocol;
}

unsigned long CProtocolMgr::GetSubProtocol(unsigned long _protocolmem)
{
	unsigned long temp_protocol;
	temp_protocol = (_protocolmem & or_subbin) >> subshift;
	return temp_protocol;
}

unsigned long CProtocolMgr::GetDetailProtocol(unsigned long _protocolmem)
{
	unsigned long temp_protocol;
	temp_protocol = (_protocolmem & or_detailbin);
	return temp_protocol;
}

bool CProtocolMgr::CheckDetailProtocol(unsigned long _protocolmem, unsigned long _protocoltype)
{
	unsigned long checkprotocol = _protocolmem & _protocoltype;
	if (checkprotocol == _protocoltype)
		return true;
	else 
		return false;
}

void CProtocolMgr::AddMainProtocol(unsigned long* _protocolmem, unsigned long _protocoltype)
{
	_protocoltype = _protocoltype << mainshift;
	*_protocolmem = *_protocolmem | _protocoltype;
}

void CProtocolMgr::AddSubProtocol(unsigned long* _protocolmem, unsigned long _protocoltype)
{
	_protocoltype = _protocoltype << subshift;
	*_protocolmem = *_protocolmem | _protocoltype;
}

void CProtocolMgr::AddDetailProtocol(unsigned long* _protocolmem, unsigned long _protocoltype)
{
	*_protocolmem = *_protocolmem | _protocoltype;
}
