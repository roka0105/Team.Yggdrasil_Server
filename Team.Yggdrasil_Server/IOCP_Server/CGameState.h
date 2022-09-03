#pragma once
#include "CState.h"
class CGameState:public CState
{
	enum class SendCompType
	{
		None,
		Max
	};
public:
	CGameState(CSession* _session) :CState(_session) 
	{
		m_sendcom_type = SendCompType::None;
	}
	virtual void Recv(t_ThreadInfo* _threadinfo)final;
	virtual void Send(t_ThreadInfo* _threadinfo)final;
	void Init();
private:
	SendCompType m_sendcom_type;
};

