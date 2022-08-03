using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using EMainProtocol = Net.Protocol.EMainProtocol;
using EProtocolType = Net.Protocol.EProtocolType;
public class LobbyManager : Singleton_Ver2.Singleton<LobbyManager>
{
    public enum ESubProtocol
    {
        None,
        Multi,
        Single,
        BackPage,
        Max
    }
    public enum EDetailProtocol
    {
        None = -1,
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
        Max
    }

    private List<RoomOutInfo> m_rooms;
    private int m_curpage;
    private int maxpage;
    public int Page
    {
        get => m_curpage;
    }
  
    #region send func
    public void LobbyEnterProcess(bool _multi)
    {
        Net.Protocol protocol;
        if (_multi)
            protocol = LobbyEnterMulti();
        else
            protocol = LobbyEnterSingle();

        Net.SendPacket packet = new Net.SendPacket();
        packet.__Initialize();
        packet.WriteProtocol(protocol.GetProtocol());
        Net.NetWorkManager.Instance.Send(packet);
    }
    private Net.Protocol LobbyEnterMulti()
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOBBY, EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.Multi, EProtocolType.Sub);
        return protocol;
    }
    private Net.Protocol LobbyEnterSingle()
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOBBY, EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.Single, EProtocolType.Sub);
        return protocol;
    }
    public void PageReqProcess(bool _next)
    {
        if (_next)
            PageReq(m_curpage + 1);
        else
            PageReq(m_curpage - 1);
    }
    private void PageReq(int _page)
    {

    }
    public void CreateRoomProcess(string _title,string _pw)
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((int)EMainProtocol.LOBBY, EProtocolType.Main);
        protocol.SetProtocol((int)ESubProtocol.Multi,EProtocolType.Sub);
        protocol.SetProtocol((int)EDetailProtocol.CreateRoom, EProtocolType.Detail);

        Net.SendPacket sendpacket = new Net.SendPacket();
        sendpacket.__Initialize();
        int size = sendpacket.Write(_title);
        size+=sendpacket.Write(_pw);
        sendpacket.WriteProtocol(protocol.GetProtocol());
        sendpacket.WriteTotalSize(size);
        Net.NetWorkManager.Instance.Send(sendpacket);
    }
    public void ChattingProcess(string _text)
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((int)EMainProtocol.LOBBY, EProtocolType.Main);
        protocol.SetProtocol((int)ESubProtocol.Multi, EProtocolType.Sub);
        protocol.SetProtocol((int)EDetailProtocol.ChatSend, EProtocolType.Detail);
        protocol.SetProtocol((int)EDetailProtocol.AllMsg, EProtocolType.Detail);

        Net.SendPacket sendpacket = new Net.SendPacket();
        sendpacket.__Initialize();
        int size = sendpacket.Write(_text);
        sendpacket.WriteProtocol(protocol.GetProtocol());
        sendpacket.WriteTotalSize(size);
        Net.NetWorkManager.Instance.Send(sendpacket);
    }
    #endregion
    #region recv func
    public void RecvProcess(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
        uint sub_protocol = _protocol.GetProtocol(EProtocolType.Sub);
        switch ((ESubProtocol)sub_protocol)
        {
            case ESubProtocol.Multi:
                MultiRecvProcess(_recvpacket, _protocol);
                break;
            case ESubProtocol.Single:
                SingleRecvProcess(_recvpacket, _protocol);
                break;
        }
    }
    public void MultiRecvProcess(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
        uint detail_protocol = _protocol.GetProtocol(EProtocolType.Raer_D);
        switch ((EDetailProtocol)detail_protocol)
        {
            case EDetailProtocol.LobbyResult:
                LobbyResult(_recvpacket, _protocol);
                break;
            case EDetailProtocol.CreateRoomResult:
                CreateRoomResult(_recvpacket, _protocol);
                break;
            case EDetailProtocol.ChatRecv:
                ChatRecv(_recvpacket, _protocol);
                break;
            case EDetailProtocol.RoomlistResult:
                RoomlistResult(_recvpacket, _protocol);
                break;
        }
    }
    public void SingleRecvProcess(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {

    }
    public void LobbyResult(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
#if UNITY_EDITOR
        Debug.Log("로비 입장");
#endif
        
    }
    public void CreateRoomResult(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
        int datasize = 0;
        bool result = false;
        _recvpacket.Read(out datasize);
        _recvpacket.Read(out result);
        if(result)
        {
#if UNITY_EDITOR
            Debug.Log("방만들기 성공!");
#endif
            MenuGUIManager.Instance.WindowActive(MenuGUIManager.EWindowType.Lobby, false);
            MenuGUIManager.Instance.WindowActive(MenuGUIManager.EWindowType.Room, true);
        }
    }
    public void ChatRecv(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
        string text;
        int datasize = 0;
        bool result = false;
        _recvpacket.Read(out datasize);
        _recvpacket.Read(out result);
        if(result)
        {
            _recvpacket.Read(out text);
            LobbyGUIManager.Instance.UpdateChat(text);
        }
        else // 채팅 보내기 실패한 경우 ex) 공백 전송
        {

        }
    }
    public void RoomlistResult(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
        EDetailProtocol pre_protocol = (EDetailProtocol)_protocol.GetProtocol(EProtocolType.Pre_D);
        //안쓸듯
        int datasize = 0;
        bool result = false;

        switch (pre_protocol)
        {
            //모든 방정보 로딩 근데 필요없을듯.
            case EDetailProtocol.AllRoom:
                break;
            //현재 페이지의 방만 로딩.
            case EDetailProtocol.PageRoom:
                //나중에 데이터 사이즈 다 빼도록 짜기 안쓰는데 현재 구조가 이렇게 짜여 있어서 
                //어쩔수 없이 한번 뽑아줘야함.
                _recvpacket.Read(out datasize);
                _recvpacket.Read(out result);
                if(result)
                {
                    _recvpacket.Read(out m_curpage);
                    _recvpacket.ReadSerialize(out m_rooms);

                    for (int i = 0; i < m_rooms.Count; i++)
                    {
                        var room = m_rooms[i];
                        LobbyGUIManager.Instance.RoomInfoSetting(i, (int)room.GetID, room.GetTitle, room.GetGameMode, room.GetCurCount, room.GetMaxEnterCount);
                    }
                }
                else
                {
                    //페이지 이동 실패.
                }
                
                break;
        }
    }
    #endregion
}
