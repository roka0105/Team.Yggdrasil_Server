#pragma once
#include "CState.h"

class CLobbyState : public CState 
{
	enum class SendCompType
	{
		None,
		BackPage,
		EnterRoom,
	};
public:
	CLobbyState(CSession* _session) :CState(_session)
	{
		m_sendcom_type = SendCompType::None;
	}
	void Init();
	virtual void Recv() final;
	virtual void Send() final;
private:
	SendCompType m_sendcom_type;
};

