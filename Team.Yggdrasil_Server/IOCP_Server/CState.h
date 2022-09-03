#pragma once

class CSession;

class CState
{
public:
	CState(CSession* _session) { m_session = _session; }
	virtual void Recv(t_ThreadInfo* _threadinfo) = 0;
	virtual void Send(t_ThreadInfo* _threadinfo) = 0;
protected:
	CSession* m_session;
};

