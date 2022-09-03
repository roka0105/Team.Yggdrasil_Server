#pragma once
#include "CState.h"

class CRoomState :public CState
{
   
public:
    enum class SendCompType
    {
        None,
        BackPage,
        EnterRoom,
        EnterGame,
        EnterLobby,
    };
public:
    CRoomState(CSession* _session):CState(_session)
    {
        m_sendcom_type = SendCompType::None;
    }
    void Init();
    virtual  void Recv(t_ThreadInfo* _threadinfo) final;
    virtual void Send(t_ThreadInfo* _threadinfo) final;
private:
    SendCompType m_sendcom_type;
};

