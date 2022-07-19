using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoomManager : Singleton_Ver2.Singleton<RoomManager>
{
    enum SUBPROTOCOL
    {
        NONE = -1,
		SetRoomInfo,
		MAX
    };
    enum DETAILPROTOCOL
    {
        NONE = -1,
		PageInfo,
        MAX
    };


    private MainThread M_MainTh;
    private ProtocolManager M_Protocol;
    //n �������� �ش��ϴ� room ������.
    private Dictionary<int,List<Room>> m_rooms;
    //�� �������� ������ ���� room ���� ��.
    private const uint m_pagelimit_room = 10;

    private delegate void Result_Process(uint _protocol , byte[] _recvdata);
    private Dictionary<SUBPROTOCOL, Result_Process> m_ResultProcess;
  
    //�� ���� ������ �Լ� �־���� (�κ��)
    //�� ���� ������ �Լ� �־���� (���ӷ��)
    private void ResultProcess(uint _protocol, byte[] _recvbuf)
    {
        uint subprotocol = M_Protocol.GetSubProtocol(_protocol);
        m_ResultProcess[(SUBPROTOCOL)subprotocol].Invoke(_protocol,_recvbuf);
    }
    private void RoomInfoResult(uint _protocol,byte[] _recvbuf)
    {
        uint detailprotocol = M_Protocol.GetDetailProtocol(_protocol);
        switch((DETAILPROTOCOL)detailprotocol)
        {
            case DETAILPROTOCOL.PageInfo:
                //unpack ����������. ������
                break;
        }
    }
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;

        m_ResultProcess = new Dictionary<SUBPROTOCOL, Result_Process>();
        m_rooms = new Dictionary<int, List<Room>>();

        M_MainTh.RecvProcess_Register((int)MAINPROTOCOL.ROOM,ResultProcess);
        
        m_ResultProcess.Add(SUBPROTOCOL.SetRoomInfo, RoomInfoResult);
    }
    public void End()
    {

    }
    void Start()
    {
        
    }

    void Update()
    {
        
    }

}
