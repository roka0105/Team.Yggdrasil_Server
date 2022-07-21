using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System;
using System.Text;
enum LobbyUseWindow
{
    None = -1,
    Menu,
    Lobby,
    CreateRoom,
    Room,
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
    [SerializeField]
    TMP_InputField m_chat_inputfield;

    Dictionary<LobbyUseWindow, GameObject> m_Windows;
    MainThread M_MainTh;
    PacketManager M_Packet;
    ProtocolManager M_Protocol;
 
    private delegate void _ResultProcess(byte[] _recvdata);
    Dictionary<DETAILPROCOTOL, _ResultProcess> m_ResultProcess;

    private int m_cur_page = 0;
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

    #region server send process
    //로비 진입에 대한 신호를 서버에 전송
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
    //방 생성 요청 신호를 서버에 전송
    public void CreateRoomSendProcess(string _name, string _pw)
    {
        uint protocol = 0;

        M_Protocol.SetMainProtocol(ref protocol, (uint)MAINPROTOCOL.LOBBY);
        M_Protocol.SetSubProtocol(ref protocol, (uint)SUBPROTOCOL.Multi);
        M_Protocol.SetDetailProtocol(ref protocol, (uint)DETAILPROCOTOL.CreateRoom);

        M_MainTh.SendQueue_Push(M_Packet.PackPacking(protocol, _name, _pw));
        MainThread.m_WaitforSendThread.Set();
    }
    public void ChatMsgSendProcess(string _msg)
    {
        uint _protocol = 0;
        M_Protocol.SetMainProtocol(ref _protocol,(uint)MAINPROTOCOL.LOBBY);
        M_Protocol.SetSubProtocol(ref _protocol, (uint)SUBPROTOCOL.Multi);
        M_Protocol.SetDetailProtocol(ref _protocol, (uint)DETAILPROCOTOL.ChatSend);
        M_Protocol.SetDetailProtocol(ref _protocol, (uint)DETAILPROCOTOL.AllMsg);

        M_MainTh.SendQueue_Push(M_Packet.PackPacking(_protocol, _msg));
        MainThread.m_WaitforSendThread.Set();
    }
    #endregion

    #region server recv process
   
    private void LobbyRecvResult(byte[] _recvdata)
    {
        Debug.Log("LobbyResult");
        //unpack
        //멀티,싱글인지에 따라 처리.
        //결과체크
        //결과
    }
    private void RoomlistRecvResult(byte[] _recvdata)
    {
        Debug.Log("RoomlistResult");
        //roommanager 호출해서 받아온 방 정보 넘기기.
    }
    private void ChatRecvResult(byte[] _recvdata)
    {
        string msg="null";
        UnPacking(_recvdata, ref msg);
        Debug.Log(msg);
    }
    #endregion

    #region client Input Func
    public void LobbyEnterBtn(bool _multi)
    {
        LobbyEnterSendProcess(_multi);
        ActiveChangeWindow(LobbyUseWindow.Lobby, LobbyUseWindow.Menu);
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
        ActiveChangeWindow(LobbyUseWindow.Room,LobbyUseWindow.CreateRoom);
        ActiveChangeWindow(LobbyUseWindow.None, LobbyUseWindow.Lobby);

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
    public void ChatSendBtn()
    {
        string msg = m_chat_inputfield.text;
        ChatMsgSendProcess(msg);
    }
    #endregion

    #region client func
    public void Init()
    {
        M_MainTh = MainThread.Instance;
        M_Protocol = ProtocolManager.Instance;
        M_Packet = PacketManager.Instance;
        m_Windows = new Dictionary<LobbyUseWindow, GameObject>();
        foreach (LobbyUseWindow type in m_window_type)
        {
            m_Windows.Add(type, m_window_objs[(int)type]);
        }
        ActiveChangeWindow(LobbyUseWindow.None, LobbyUseWindow.CreateRoom);

        ServerInit();
    }
    //윈도우 창 활성화 비활성화 근데 이거 그냥 windowmanager에 빼놓기.
    private void ActiveChangeWindow(LobbyUseWindow _truewindow, LobbyUseWindow _falsewindow)
    {
        if (_truewindow != LobbyUseWindow.None)
            m_Windows[_truewindow].SetActive(true);
        if (_falsewindow != LobbyUseWindow.None)
            m_Windows[_falsewindow].SetActive(false);
    }
    #endregion

    #region server func
    private void ServerInit()
    {
        M_MainTh.RecvProcess_Register((int)MAINPROTOCOL.LOBBY, ResultProcess);
        m_ResultProcess = new Dictionary<DETAILPROCOTOL, _ResultProcess>();
        m_ResultProcess.Add(DETAILPROCOTOL.LobbyResult, LobbyRecvResult);
        m_ResultProcess.Add(DETAILPROCOTOL.RoomlistResult, RoomlistRecvResult);
        m_ResultProcess.Add(DETAILPROCOTOL.ChatRecv, ChatRecvResult);
    }
    //서버에서 받아온 recv packet 을 처리해서 패킷에 대한 결과를 처리한다.
    private void ResultProcess(uint _protocol, byte[] _recvbuf)
    {
        uint detailprotocol = M_Protocol.GetPreDetailProtocol(_protocol);
        m_ResultProcess[(DETAILPROCOTOL)detailprotocol]?.Invoke(_recvbuf);
    }
    #endregion
    void UnPacking(byte[] _recvdata,ref string _msg)
    {
        int index = sizeof(uint) + sizeof(int);
        int strsize = 0;
        strsize = BitConverter.ToInt32(_recvdata, index);
        index += sizeof(int);
        _msg = Encoding.Unicode.GetString(_recvdata, index, strsize);
    }
}
