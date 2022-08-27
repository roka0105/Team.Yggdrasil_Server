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
	virtual void Recv()final;
	virtual void Send()final;
	void Init();
private:
	SendCompType m_sendcom_type;
};

