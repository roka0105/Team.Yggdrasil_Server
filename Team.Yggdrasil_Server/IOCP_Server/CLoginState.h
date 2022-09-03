#pragma once

#include "CState.h"

class CLoginState :public CState
{
public:
	CLoginState(CSession* _session) :CState(_session) { is_lobby = false; }
    void Init()
    {
        is_lobby = false;
    }
	virtual void Recv(t_ThreadInfo* _threadinfo) final;
	virtual void Send(t_ThreadInfo* _threadinfo) final;
private:
	bool is_lobby;
};

