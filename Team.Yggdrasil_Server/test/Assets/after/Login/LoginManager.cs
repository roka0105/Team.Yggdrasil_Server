using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;
using EMainProtocol = Net.Protocol.EMainProtocol;
using EProtocolType = Net.Protocol.EProtocolType;
public class LoginManager : Singleton_Ver2.Singleton<LoginManager>
{
    public enum ESubProtocol
    {
        None,
        LoginInfo,
        LoginResult,
        JoinInfo,
        JoinResult,
        LogoutInfo,
        LogoutResult,
        Max,
    }
    enum Result
    {
        Fail = 0,
        Suc = 1
    }
    private LoginInfo m_curinfo;
    #region send func
    public void LoginProcess(LoginInfo _info)
    {
        m_curinfo = _info;

        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOGIN,EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.LoginInfo, EProtocolType.Sub);
        //protocol.LittleEndian();
        Net.SendPacket packet = new Net.SendPacket();
        packet.__Initialize();
        int totalsize=packet.Write(_info);
        packet.WriteTotalSize(totalsize);
        packet.WriteProtocol(protocol.GetProtocol());
        Net.NetWorkManager.Instance.Send(packet);
    }
    public void LogoutProcess()
    {
        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOGIN, EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.LogoutInfo, EProtocolType.Sub);

        Net.SendPacket packet = new Net.SendPacket();
        packet.__Initialize();
        packet.WriteProtocol(protocol.GetProtocol());
        Net.NetWorkManager.Instance.Send(packet);
    }
    public void JoinProcess(LoginInfo _info)
    {
        m_curinfo = _info;

        Net.Protocol protocol = new Net.Protocol();
        protocol.SetProtocol((uint)EMainProtocol.LOGIN, EProtocolType.Main);
        protocol.SetProtocol((uint)ESubProtocol.JoinInfo, EProtocolType.Sub);

        Net.SendPacket packet = new Net.SendPacket();
        packet.__Initialize();
        int totalsize = packet.Write(_info);
        packet.WriteTotalSize(totalsize);
        packet.WriteProtocol(protocol.GetProtocol());
        Net.NetWorkManager.Instance.Send(packet);
    }
    public void RemoveInfoProcess()
    {

    }
    #endregion
    #region recv func
    public void RecvProcess(Net.RecvPacket _recvPacket, Net.Protocol _protocol)
    {
        ESubProtocol subprotocol = (ESubProtocol)_protocol.GetProtocol(Net.Protocol.EProtocolType.Sub);
        switch (subprotocol)
        {
            case ESubProtocol.LoginResult:
                LoginResult(_recvPacket, _protocol);
                break;
            case ESubProtocol.JoinResult:
                JoinResult(_recvPacket, _protocol);
                break;
            case ESubProtocol.LogoutResult:
                LogoutResult(_recvPacket, _protocol);
                break;
        }
    }
    //packet 구조  allsize/packetno/protocol/datasize/data(bool=1byte,string)
    private void LoginResult(Net.RecvPacket _recvPacket, Net.Protocol _protocol)
    {
        byte result = 0;
        string msg;
        int datasize = 0;
        _recvPacket.Read(out datasize); /*여기서는 안씀*/
        _recvPacket.Read(out result);
        _recvPacket.Read(out msg);
#if UNITY_EDITOR
        Debug.Log(msg);
#endif
        switch ((Result)Convert.ToInt32(result))
        {
            case Result.Fail:
                //로그인 실패시 임시로 저장해둔 로그인 정보 지우기.
                m_curinfo.Clear();
                break;
            case Result.Suc:
                //성공했을때 ui가 해야할일 해당 uimanager 호출해서 하기.

                break;
        }
        LoginGUIManager.Instance.OnClick_Exit(true);
        MenuGUIManager.Instance.MenuChange(MenuGUIManager.EMenuModeType.Menu);
    }
    private void JoinResult(Net.RecvPacket _recvPacket, Net.Protocol _protocol)
    {
        byte result = 0;
        string msg;
        int datasize = 0;
        _recvPacket.Read(out datasize); /*여기서는 안씀*/
        _recvPacket.Read(out result);
        _recvPacket.Read(out msg);
#if UNITY_EDITOR
        Debug.Log(msg);
#endif
        switch ((Result)result)
        {
            case Result.Fail:
                break;
            case Result.Suc:
                //성공했을때 ui가 해야할일 해당 uimanager 호출해서 하기.
                break;
        }
        LoginGUIManager.Instance.OnClick_Exit(false);
    }
    private void LogoutResult(Net.RecvPacket _recvPacket, Net.Protocol _protocol)
    {
        string msg;
        int datasize = 0;
        _recvPacket.Read(out datasize); /*여기서는 안씀*/
        _recvPacket.Read(out msg);
#if UNITY_EDITOR
        Debug.Log(msg);
#endif
        MenuGUIManager.Instance.MenuChange(MenuGUIManager.EMenuModeType.Login);
    }

    #endregion

}
