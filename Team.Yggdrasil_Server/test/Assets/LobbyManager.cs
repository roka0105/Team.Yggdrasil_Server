using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
enum LobbyUseWindow
{
    None = -1,
    Menu,
    CreateRoom,
    Max
}
public class LobbyManager : Singleton_Ver2.Singleton<LobbyManager>
{
    [SerializeField]
    List<LobbyUseWindow> m_window_type;
    [SerializeField]
    List<GameObject> m_window_objs;
    [SerializeField]
    List<TMP_InputField> m_create_inputfields;

    Dictionary<LobbyUseWindow, GameObject> m_Windows;
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
        NONE = -1,
        //========상위=========
        LobbyEnter = 1,
        LobbyResult = 2,
        CreateRoom = 4,
        CreateRoomResult = 8,
        ChatSend = 16,
        ChatRecv = 32,
        RoomlistUpdate = 64,
        RoomlistResult = 128,
        //========하위=========
        NoticeMsg = 256,//공지 메세지 (운영자가 전송)
        AllMsg = 512,//전체 메세지 (일반 유저들이 사용)
        AllRoom = 1024,
        PageRoom = 2048,
        MAX
    }
    public void LobbyEnterSendProcess(bool _multi)
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
        M_MainTh.SendQueue_Push(M_Packet.PackPacking(protocol));
        MainThread.m_WaitforSendThread.Set();
    }
    public void CreateRoomSendProcess(string _name,string _pw)
    {
        uint protocol = 0;

        M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.ROOM);
        M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.Multi);
        M_Protocol.SetDetailProtocol(ref protocol, (uint)DETAILPROCOTOL.CreateRoom);

        M_MainTh.SendQueue_Push(M_Packet.PackPacking(protocol, _name, _pw));
        MainThread.m_WaitforSendThread.Set();
    }
    public void CreateRoomBtn()
    {
        ActiveChangeWindow(LobbyUseWindow.CreateRoom, LobbyUseWindow.None);
    }
    public void CreateRoomOkBtn()
    {
        // 입력값 전송
        string NAME = m_create_inputfields[0].text;
        string PW = m_create_inputfields[1].text;
        CreateRoomSendProcess(NAME, PW);
    }
    public void CreateRoomCancelBtn()
    {
        ActiveChangeWindow(LobbyUseWindow.None, LobbyUseWindow.CreateRoom);
    }
    //메뉴로 돌아가기.
    public void BackMenuBtn()
    {
        Debug.Log("backmenu");
    }
    public void ChagePageBtn(bool _next)
    {

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
        m_ResultProcess = new Dictionary<DETAILPROCOTOL, _ResultProcess>();
        m_ResultProcess.Add(DETAILPROCOTOL.LobbyResult, LobbyResult);
        m_ResultProcess.Add(DETAILPROCOTOL.RoomlistResult, RoomlistResult);
        M_MainTh.RecvProcess_Register((int)MAINPROTOCOL.LOBBY, ResultProcess);

        foreach(LobbyUseWindow type in m_window_type)
        {
            m_Windows.Add(type, m_window_objs[(int)type]);
        }
        ActiveChangeWindow(LobbyUseWindow.None, LobbyUseWindow.CreateRoom);
    }

    private void ActiveChangeWindow(LobbyUseWindow _truewindow, LobbyUseWindow _falsewindow)
    {
        if (_truewindow != LobbyUseWindow.None)
            m_Windows[_truewindow].SetActive(true);
        if (_falsewindow != LobbyUseWindow.None)
            m_Windows[_falsewindow].SetActive(false);
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
