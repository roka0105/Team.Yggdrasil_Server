using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyManager : Singleton_Ver2.Singleton<LobbyManager>
{
    MainThread M_MainTh;
    PacketManager M_Packet;
    ProtocolManager M_Protocol;
    private delegate void _ResultProcess(byte[] _recvdata);
    Dictionary<DETAILPROCOTOL, _ResultProcess> m_ResultProcess;
    enum SUBPROTOCOL
    {
        NONE,
        Multi,
        Sigle,
        BackPage,
        MAX
    }
    enum DETAILPROCOTOL
    {
        NONE=-1,
        //========����=========
        LobbyEnter = 1,
        LobbyResult = 2,
        CreateRoom = 4,
        CreateRoomResult = 8,
        ChatSend = 16,
        ChatRecv = 32,
        RoomlistUpdate = 64,
        RoomlistResult = 128,
        //========����=========
        NoticeMsg = 256,//���� �޼��� (��ڰ� ����)
        AllMsg = 512,//��ü �޼��� (�Ϲ� �������� ���)
        AllRoom = 1024,
        PageRoom = 2048,
        MAX
    }
    public void LobbyEnterProcess(bool _multi)
    {
        Debug.Log("lobbyenter");
        uint protocol = 0;
        M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOBBY);
        if (_multi)
        {
            M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.Multi);
        }
        else
        {
            M_Protocol.SetDetailProtocol(ref protocol, (uint)SUBPROTOCOL.Sigle);
        }
           
        M_Protocol.SetDetailProtocol(ref protocol, (uint)DETAILPROCOTOL.LobbyEnter);

        byte[] senddata = M_Packet.PackPacking(protocol);
        M_MainTh.SendQueue_Push(senddata);
        MainThread.m_WaitforSendThread.Set();
    }
    private void ResultProcess(uint _protocol, byte[] _recvbuf)
    {
        uint detailprotocol = M_Protocol.GetDetailProtocol(_protocol);
        m_ResultProcess[(DETAILPROCOTOL)detailprotocol]?.Invoke(_recvbuf);
    }
    private void LobbyResult(byte[] _recvdata)
    {
        Debug.Log("LobbyResult");
        //unpack
        //��Ƽ,�̱������� ���� ó��.
        //���üũ
        //���
    }
    private void RoomlistResult(byte[] _recvdata)
    {
        Debug.Log("RoomlistResult");
        //roommanager ȣ���ؼ� �޾ƿ� �� ���� �ѱ��.
    }
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;
        M_Packet = PacketManager.Instance;
        m_ResultProcess = new Dictionary<DETAILPROCOTOL, _ResultProcess>();
        m_ResultProcess.Add(DETAILPROCOTOL.LobbyResult, LobbyResult);
        m_ResultProcess.Add(DETAILPROCOTOL.RoomlistResult, RoomlistResult);
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
