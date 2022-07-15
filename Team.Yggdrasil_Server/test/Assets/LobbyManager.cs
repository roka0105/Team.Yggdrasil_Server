using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyManager : Singleton_Ver2.Singleton<LobbyManager>
{
    MainThread M_MainTh;
    PacketManager M_Packet;
    ProtocolManager M_Protocol;
    private delegate void _ResultProcess(byte[] _recvdata);
    Dictionary<SUBPROTOCOL, _ResultProcess> m_ResultProcess;
    enum SUBPROTOCOL
    {
        NONE,
        LobbyEnter,
        LobbyResult,
        CreateRoom,
        CreateRoomResult,
        ChatSend,
        ChatRecv,
        RoomlistUpdate,
        RoomlistResult,
      
        Max
    }
    enum DETAILPROCOTOL
    {
        NONE=-1,
        Multi=1,
        Sigle=2,
        NoticeMsg=4,//공지 메세지 (운영자가 전송)
        AllMsg=8,//전체 메세지 (일반 유저들이 사용)
        AllRoom=16,
        PageRoom=32,
        MAX
    }
    public void LobbyEnterProcess(bool _multi)
    {
        Debug.Log("lobbyenter");
        uint protocol = 0;
        M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOBBY);
        M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.LobbyEnter);
        if (_multi)
        {
            M_Protocol.SetDetailProtocol(ref protocol, (uint)DETAILPROCOTOL.Multi);
        }
        else
        {
            M_Protocol.SetDetailProtocol(ref protocol, (uint)DETAILPROCOTOL.Sigle);
        }

        byte[] senddata = M_Packet.PackPacking(protocol);
        M_MainTh.SendQueue_Push(senddata);
        MainThread.m_WaitforSendThread.Set();
    }
    private void ResultProcess(uint _protocol, byte[] _recvbuf)
    {
        uint subprotocol = M_Protocol.GetSubProtocol(_protocol);
        m_ResultProcess[(SUBPROTOCOL)subprotocol]?.Invoke(_recvbuf);
    }
    private void LobbyResult(byte[] _recvdata)
    {
        Debug.Log("LobbyResult");
        //unpack
        //멀티,싱글인지에 따라 처리.
        //결과체크
        //결과
    }
    private void RoomlistResult(byte[] _recvdata)
    {
        Debug.Log("RoomlistResult");
        //roommanager 호출해서 받아온 방 정보 넘기기.
    }
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;
        M_Packet = PacketManager.Instance;
        m_ResultProcess = new Dictionary<SUBPROTOCOL, _ResultProcess>();
        m_ResultProcess.Add(SUBPROTOCOL.LobbyResult, LobbyResult);
        m_ResultProcess.Add(SUBPROTOCOL.RoomlistResult, RoomlistResult);
        M_MainTh.RecvProcess_Register((int)MAINPROTOCOL.LOBBY, ResultProcess);
    }
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }
}
