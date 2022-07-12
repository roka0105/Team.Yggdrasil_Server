#pragma once
#include "CState.h"
#include "CSession.h"

#include "CProtocolMgr.h"
class CLobbyState : public CState 
{
public:
	CLobbyState(CSession* _session) :CState(_session)
	{

	}
	virtual void Recv() final;
	virtual void Send() final;
};

