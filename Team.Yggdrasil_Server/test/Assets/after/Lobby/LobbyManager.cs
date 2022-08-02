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
        //========하위=========post
        NoticeMsg = 256,//공지 메세지 (운영자가 전송)
        AllMsg = 512,//전체 메세지 (일반 유저들이 사용)
        AllRoom = 1024,
        PageRoom = 2048,
        Max
    }

    private List<RoomOutInfo> m_rooms;

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
    public Net.Protocol LobbyEnterMulti()
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOBBY, EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.Multi, EProtocolType.Sub);
        return protocol;
    }
    public Net.Protocol LobbyEnterSingle()
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOBBY, EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.Single, EProtocolType.Sub);
        return protocol;
    }
    #endregion
}
