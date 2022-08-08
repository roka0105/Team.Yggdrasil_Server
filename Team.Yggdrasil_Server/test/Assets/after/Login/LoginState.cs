using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
namespace Net
{
    public class LoginState : IState
    {
        private NetSession m_client;
        private IState.State m_state;
        public NetSession Client 
        { 
            get => m_client; 
            set => m_client=value;
        }
        public IState.State ClientState => m_state;

        public LoginState(NetSession _session)
        {
            m_state = IState.State.Login;
            this.Client = _session;
        }
       

        public void RecvComplete(RecvPacket _recvpacket)
        {
            uint protocol=0;
            _recvpacket.Read(out protocol);
            Protocol protocol_manager = new Protocol(Convert.ToUInt32(protocol));
            switch((Protocol.EMainProtocol)protocol_manager.GetProtocol(Protocol.EProtocolType.Main))
            {
                case Protocol.EMainProtocol.LOGIN:
                    LoginManager.Instance.RecvProcess(_recvpacket,protocol_manager);
                    break;
                case Protocol.EMainProtocol.LOBBY:
                    //현재 state가 login 일때 lobby로 들어오는 경우는 
                    //1. init 초기값 세팅을 위한 전송
                    //2. multi 로비 입장 버튼 입력시 
                    //위의 두가지 경우만 존재. multi 요청일 시에만 lobby state로 변하기 때문에
                    //if으로 구분 함.
                    if((LobbyManager.ESubProtocol)protocol_manager.GetProtocol(Protocol.EProtocolType.Sub)
                        ==LobbyManager.ESubProtocol.Multi)
                    m_client.SetState(m_client.m_Lobbystate);
                    LobbyManager.Instance.RecvProcess(_recvpacket, protocol_manager);
                    break;
            }
        }

        public void SendComplete()
        {
           
        }

        public void OnChaged()
        {
            
        }
        
        
    }

}
