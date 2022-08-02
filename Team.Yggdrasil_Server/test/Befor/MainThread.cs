using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;
using System;

namespace Befor
{
    enum STATE
    {
        NONE,
        LOGIN,
        LOBBY,
        GAME,
        MAX
    }
    enum MAINPROTOCOL
    {
        NONE,
        LOGIN,
        LOBBY,
        ROOM,
        GAME,
        MAX
    }
    public class MainThread : Singleton_Ver2.Singleton<MainThread>
    {

        struct ThreadArgs
        {
            public ClientManager M_Client;
            public Queue<Byte[]> queue;
        }
        #region manager
        ClientManager M_Client;
        MouseManager M_Mouse;
        PacketManager M_Packet;
        MenuManager M_Menu;
        LoginManager M_Login;
        LobbyManager M_Lobby;
        RoomManager M_Room;
        ProtocolManager M_Protocol;
        #endregion

        #region server instance
        public static EventWaitHandle m_WaitforSendThread;
        private List<STATE> statelist;
        private Queue<Byte[]> m_send_queue;
        private Queue<Byte[]> m_recv_queue;
        private Thread m_sendThread;
        private Thread m_recvThread;
        public delegate void RecvProcess(uint _protocol, Byte[] _packet);
        private Dictionary<MAINPROTOCOL, RecvProcess> m_RecvProcess;
        private STATE m_curState;
        #endregion

        private bool is_loging;

        #region server recv func
        public void RecvProcess_Register(uint _mainprotocol, RecvProcess _recvprocess)
        {
            m_RecvProcess.Add((MAINPROTOCOL)_mainprotocol, _recvprocess);
        }
        public void RecvQeue_Push(Byte[] _recvdata)
        {
            m_recv_queue.Enqueue(_recvdata);
        }
        public Byte[] RecvQueue_Pop()
        {
            return m_recv_queue.Dequeue();
        }
        void RecvThread(ThreadArgs args)
        {
            bool flag = false;
            while (true)
            {
                Byte[] recvbuf;
                flag = args.M_Client.Recv(out recvbuf);
                if (flag)
                    args.queue.Enqueue(recvbuf);
            }
        }

        private void Recv()
        {
            if (m_recv_queue == null)
            {
                Debug.Log("recvqueue null");
                return;
            }
            if (m_recv_queue.Count > 0)
            {
                //패킷 빼가서 동작.
                Byte[] databuf = m_recv_queue.Dequeue();
                uint protocol = M_Packet.GetPROTOCOL(databuf);
                //프로토콜 매니저를 통해서 mainprotocol 추출해서 밑에 넣기.
                //mainprotocol은 사용할 매니저를 표현합니다.
                //서버에서 온 코드가 어느 매니저에서 작동할 건지 정보를 담고 있습니다.
                //해당 매니저에서 무슨 일을 할 것인지 좀 더 세부적인 데이터는 subprotocol을 보고 결정합니다.
                MAINPROTOCOL main_protocol = (MAINPROTOCOL)M_Protocol.GetMainProtocol(protocol);

                m_RecvProcess[main_protocol]?.Invoke(protocol, databuf);
            }
        }

        #endregion

        #region server send func
        public void SendQueue_Push(Byte[] _senddata)
        {
            m_send_queue.Enqueue(_senddata);
        }
        public Byte[] SendQueue_Pop()
        {
            return m_send_queue.Dequeue();
        }
        void SendThread(ThreadArgs args)
        {
            bool flag = false;
            while (true)
            {
                m_WaitforSendThread.WaitOne();
                if (args.queue.Count > 0)
                {
                    Byte[] sendbuf = args.queue.Dequeue();
                    flag = args.M_Client.Send(sendbuf);
                }
            }
        }
        #endregion

        #region server func
        private void ServerInit()
        {
            M_Client.Init();

            m_send_queue = new Queue<byte[]>();
            m_recv_queue = new Queue<byte[]>();
            m_RecvProcess = new Dictionary<MAINPROTOCOL, RecvProcess>();
            m_WaitforSendThread = new EventWaitHandle(false, EventResetMode.AutoReset);

            M_Client.ConnectToTcpServer();
            CreateServerThread();
        }
        private void CreateServerThread()
        {
            ThreadArgs th1_param;
            th1_param.M_Client = M_Client;
            th1_param.queue = m_send_queue;
            ThreadArgs th2_param;
            th2_param.M_Client = M_Client;
            th2_param.queue = m_recv_queue;
            m_sendThread = new Thread(() => SendThread(th1_param));
            m_recvThread = new Thread(() => RecvThread(th2_param));
            m_sendThread.Start();
            m_recvThread.Start();
        }

        private void ServerEnd()
        {
            if (m_sendThread != null)
            {
                m_sendThread.Abort();
            }
            if (m_recvThread != null)
            {
                m_recvThread.Abort();
            }
            M_Client.Dispose();
            M_Client.End();
        }

        #endregion
        public bool GS_Loging
        {
            get => is_loging;
            set => is_loging = value;
        }

        private void Create()
        {
            M_Client = ClientManager.Instance;
            M_Mouse = MouseManager.Instance;
            M_Packet = PacketManager.Instance;
            M_Protocol = ProtocolManager.Instance;
            M_Menu = MenuManager.Instance;
            M_Login = LoginManager.Instance;
            M_Lobby = LobbyManager.Instance;
            M_Room = RoomManager.Instance;
        }

        private void Init()
        {

            is_loging = false;


            ServerInit();


            M_Mouse.Init();
            M_Menu.Init();
            M_Login.Init();
            M_Lobby.Init();
            M_Room.Init();


        }
        private void End()
        {
            ServerEnd();
        }

        // Start is called before the first frame update
        void Start()
        {
            Create();
            Init();
        }
        // Update is called once per frame
        void Update()
        {
            Recv();

        }
        private void OnApplicationQuit()
        {
            End();
        }

    }
}
// RecvProcess 메인 프로토콜 분해해서 해당 작업 수행하는 함수 연결하는거 다시 고치기 