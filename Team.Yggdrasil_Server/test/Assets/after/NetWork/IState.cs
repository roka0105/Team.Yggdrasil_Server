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
            //로그인
            Login,
            //로비
            Lobby,
            //룸
            Room,
            //게임
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

