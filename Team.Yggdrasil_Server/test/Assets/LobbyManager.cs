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
        NONE = -1,
        LobbyResult,
        Max
    }
    enum DERAILPROCOTOL
    {
        NONE=-1,
        Multi,
        Sigle,
        MAX
    }
    public void LobbyEnterProcess(bool _multi)
    {
        Debug.Log("lobbyenter");
        if (_multi)
        {
            uint protocol = 0;
            M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOBBY);
          

            byte[] senddata = M_Packet.PackPacking(protocol);
            M_MainTh.SendQueue_Push(senddata);
            MainThread.m_WaitforSendThread.Set();
           
        }
        else
        {

        }


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
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;
        M_Packet = PacketManager.Instance;
        m_ResultProcess = new Dictionary<SUBPROTOCOL, _ResultProcess>();
        m_ResultProcess.Add(SUBPROTOCOL.LobbyResult, LobbyResult);
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
