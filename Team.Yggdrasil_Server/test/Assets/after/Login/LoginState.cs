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
