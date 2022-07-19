#pragma once

#include "CState.h"

class CLoginState :public CState
{
public:
	CLoginState(CSession* _session) :CState(_session) { is_lobby = false; }
	virtual void Recv() final;
	virtual void Send() final;
private:
	bool is_lobby;
};

