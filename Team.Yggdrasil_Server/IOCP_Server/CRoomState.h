#pragma once
#include "CState.h"
class CRoomState :
    public CState
{
public:
    CRoomState(CSession* _session):CState(_session)
    {
    }
    virtual  void Recv() final;
    virtual void Send() final;
private:
};

