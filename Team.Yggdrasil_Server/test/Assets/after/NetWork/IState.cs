using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Net
{
    public interface IState
    {
        public enum State
        {
            None,
            //�α���
            Login,
            //�κ�
            Lobby,
            //��
            Room,
            //����
            Game,
            Max

        }
        public abstract NetSession Client { get; set;}
        public abstract State ClientState { get; }
        public abstract void SendComplete();
        public abstract void RecvComplete(RecvPacket _recvpacket);
        public abstract void OnChaged();
    }
}

