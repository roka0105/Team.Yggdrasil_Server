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
        Debug.Log("�κ� ����");
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
            Debug.Log("�游��� ����!");
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
        else // ä�� ������ ������ ��� ex) ���� ����
        {

        }
    }
    public void RoomlistResult(Net.RecvPacket _recvpacket, Net.Protocol _protocol)
    {
        EDetailProtocol pre_protocol = (EDetailProtocol)_protocol.GetProtocol(EProtocolType.Pre_D);
        //�Ⱦ���
        int datasize = 0;
        bool result = false;

        switch (pre_protocol)
        {
            //��� ������ �ε� �ٵ� �ʿ������.
            case EDetailProtocol.AllRoom:
                break;
            //���� �������� �游 �ε�.
            case EDetailProtocol.PageRoom:
                //���߿� ������ ������ �� ������ ¥�� �Ⱦ��µ� ���� ������ �̷��� ¥�� �־ 
                //��¿�� ���� �ѹ� �̾������.
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
                    //������ �̵� ����.
                }
                
                break;
        }
    }
    #endregion
}
